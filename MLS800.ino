#include "MLS800.h"
#include <Wire.h>
#include <MCP23017.h>
#include <SAA1064.h>
#include <E24LC256.h>
#include <MIDI.h>

MCP23017 _ui = MCP23017(UI_ADDR);
E24LC256 _storage = E24LC256(STORAGE_ADDR);
SAA1064 _display = SAA1064(DISPLAY_ADDR);
MCP23017 _loops = MCP23017(LOOP_ADDR);

PatchManager _patchMngr = PatchManager(&_storage);
Config _config;

DeviceState _mode;
byte _editingValue;

byte _currentLoopStates;
byte _currentInputStates;
byte _currentEditBttnState;

byte _currentEditValue;

long _previousMillis;
long _previousEditLedMillis;
long _previousDisplayMillis;
bool _ledsShuttedOff = false;
bool _displayShuttedOff = true;
bool _blinkLoopStates;
bool _blinkEditLed;
bool _blinkDisplay;

volatile bool input = false;
volatile bool edit = false;

#ifdef _DEBUG
#define debugPrintln(msg) startSerial();Serial.println(msg);endSerial();
#define debugPrint(msg) startSerial();Serial.print(msg);endSerial();
void startSerial()
{
	Serial.end();
	Serial.begin(115200);
}

void endSerial()
{
	Serial.flush();
	Serial.end();
	MIDI.begin(MIDI_CHANNEL_OMNI);
}
#else
#define debugPrintln
#define debugPrint
#endif

//interrupt handler for user input
void uiInterrupt()
{
	input = true;
}

void editInterrupt()
{
	edit = true;
}

//debounce user input and clear interrupts
byte debounceInput()
{
	byte a, b;
	byte captureA, captureB;
	byte currentB;

	delay(DEBOUNCE_DELAY);
	input = false;
	_ui.interruptedBy(a, b);
	_ui.clearInterrupts(captureA, captureB);
	currentB = _ui.readPort(UI_BTN_PORT);
	if((b & ~currentB) == (b & ~captureB) && currentB != _currentInputStates)
	{
		_currentInputStates = currentB;
		if(b & ~currentB) return b;
	}

	return LOW;
}

byte debounceEdit()
{
	byte editBttnState = digitalRead(EDIT_BTTN_PIN);
	if(editBttnState != _currentEditBttnState)
	{
		edit = false;
		delay(DEBOUNCE_DELAY);
		_currentEditBttnState = digitalRead(EDIT_BTTN_PIN);
		return _currentEditBttnState;
	}

	//return actual state only on debounced RISING
	return LOW;
}

//==================== /SETUP =======================//

//storage configuration
void setupStorage()
{
	//nothing to do yet
}

//patch manager configuration
void setupPatchManager()
{
	_patchMngr.init(PATCHES_ADDR, PATCH_COUNT, CC_COUNT);
}


//display configuration
void setupDisplay(SAA1064_DIM dim)
{
	_display.init(DYNAMIC, dim);
}

//ui configuration
void setupUi()
{
	_ui.init();

	//port a : ouput
	_ui.portMode(UI_LEDS_PORT, 0x00);

	//port b : input
	_ui.portMode(UI_BTN_PORT, 0xFF);

	_ui.interruptMode(OR);
	_ui.interrupt(UI_BTN_PORT, CHANGE);

	_ui.clearInterrupts();
	attachInterrupt(UI_INT, uiInterrupt, CHANGE);

	//raw arduino pins configuration
	pinMode(EDIT_BTTN_PIN, INPUT);
	attachInterrupt(EDIT_INT, editInterrupt, RISING);

	pinMode(EDIT_LED_PIN, OUTPUT);
}

void setupLoops()
{
	_loops.init();
	//all ports to output
	//port A : OFF port
	_loops.portMode(0, 0x00);
	//port B : ON port
	_loops.portMode(1, 0x00);
}

void setupMidi()
{
	MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.setHandleControlChange(handleControlChange);
	MIDI.setHandleProgramChange(handleProgramChange);
}

//===================== /SETUP =======================//

//====================== MIDI ========================//

void handleProgramChange(byte channel, byte number)
{
	debugPrint("Program Change : ");
	debugPrintln(number);
	if(_mode == PLAYING)
	{
		_config.patchNumber = number;
		loadPatch(_config.patchNumber);
	}
	else if(_mode == LEARNING) 
	{
		_config.patchNumber = number;
		swichState();
	}
}

void handleControlChange(byte channel, byte number, byte value)
{
	debugPrint("Control Change : ");
	debugPrint(number);
	debugPrint(" ");
	debugPrintln(value);
}

//====================== MIDI ========================//

//====================== LEDS ========================//

void blinkEditLed()
{
	long currentMillis = millis();
	if(currentMillis - _previousEditLedMillis > BLINK_DELAY_HIGH) {
		digitalWrite(EDIT_LED_PIN, !digitalRead(EDIT_LED_PIN));
		_previousEditLedMillis = currentMillis;
	}
}

void blinkDisplay()
{
	long currentMillis = millis();
	if(currentMillis - _previousDisplayMillis > BLINK_DELAY_HIGH) {
		if(_displayShuttedOff) displayPatchNumber(_config.patchNumber);
		else _display.clear();

		_displayShuttedOff = !_displayShuttedOff;
		_previousDisplayMillis = currentMillis;
	}
}

void stopBlinkDisplay()
{
	_blinkDisplay = false;
	displayPatchNumber(_config.patchNumber);
	_displayShuttedOff = true;
}

void displayLoopStates(byte state)
{
	_ui.writePort(UI_LEDS_PORT, state);
}

void blinkLoopStates()
{
	long currentMillis = millis();
	short delay = _ledsShuttedOff ? BLINK_DELAY_LOW : BLINK_DELAY_HIGH;
	if(currentMillis - _previousMillis > delay) {
		if(_ledsShuttedOff) displayLoopStates(_currentLoopStates);
		else displayLoopStates(0);

		_ledsShuttedOff = !_ledsShuttedOff;
		_previousMillis = currentMillis;
	}
}

void stopBlinkLoopStates()
{
	_blinkLoopStates = false;
	displayLoopStates(_currentLoopStates);
	_ledsShuttedOff = false;
}

//====================== /LEDS ======================//

//===================== LOOPS =======================//

void applyLoopStates(byte state)
{
	_loops.write(~state | state << 8);
	delay(7);
	//lay down the previous impulse
	_loops.write(0x0000);
}


//==================== /LOOPS =======================//

//basic config management
void writeConfig()
{
	_storage.writeBlock(CONFIG_ADDR, _config);
}

void resetConfig()
{
	Config config = {
		CONFIG_SEED,
		MLS800_VERSION,
		MIDI_CHANNEL_OMNI,
		MIDI_CHANNEL_OFF,
		0,
		0,
		(byte)MID
	};

	_config = config;
	writeConfig();
}

void readConfig()
{
	_storage.readBlock(CONFIG_ADDR, _config);
	if(!String(CONFIG_SEED).equals(_config.seed)) {
		resetConfig();
	}
}
//end basic config management


//===================== PATCHES ======================//

void applyPatch(byte patch)
{
	displayLoopStates(patch);
	applyLoopStates(patch);
}

byte loadPatch(byte patchNumber)
{
	byte patch;
	_patchMngr.load(patchNumber, patch);
	_config.patchNumber = patchNumber;
	_config.currentState = patch;

	writeConfig();

	applyPatch(_config.currentState);
	displayPatchNumber(_config.patchNumber);

	return _config.currentState;
}

//==================== /PATCHES =======================//


//====================== MODES =======================//

void startLearning()
{
	_blinkEditLed = true;
	_blinkDisplay = true;
	_currentEditValue = _config.currentState;
}

void endLearning()
{
	displayPatchNumber(_config.patchNumber);
}

void startEditing()
{
	_blinkEditLed = false;
	stopBlinkDisplay();
	_currentEditValue = _config.currentState;
	digitalWrite(EDIT_LED_PIN, HIGH);
}

void endEditing()
{
	_config.currentState = _currentEditValue;
	writeConfig();
	_patchMngr.save(_config.patchNumber, _config.currentState);
	digitalWrite(EDIT_LED_PIN, LOW);
}

void swichState() {
	switch(_mode)
	{
	case PLAYING:
		_mode = LEARNING;
		startLearning();
		break;
	case LEARNING:
		endLearning();
		_mode = EDITING;
		startEditing();
		break;
	case EDITING:
		endEditing();
		_mode = PLAYING;
		break;
	}
	debugPrintln("Device State changed :");
	debugPrintln(_mode == PLAYING ? "PLAYING" : _mode == LEARNING ? "LEARNING" : "EDITING");
}

//====================== /MODES ======================//

//===================== DISPLAY ======================//

void printNumber(char * s, byte nb)
{
	itoa(nb, nb > 99 ? s + 1 : nb > 9 ? s + 2 : s + 3, 10);
}

void displayPatchNumber(byte nb)
{
	char str[5] = "P   ";
	printNumber(str, nb);
	_display.display(str);
}

//===================== /DISPLAY =====================//

void setup()
{
	Wire.begin();
	debugPrintln("Setup storage...");
	setupStorage();
	debugPrintln("Reading configuration...");
	readConfig();
	debugPrintln("Setup patch manager...");
	setupPatchManager();
	debugPrintln("Setup display...");
	setupDisplay((SAA1064_DIM)_config.displayDim);
	_display.display(_config.version);
	
	debugPrintln("Setup user interface...");
	setupUi();
	debugPrintln("Setup loops...");
	setupLoops();

#ifdef _DEBUG
	_ui.debug();
#endif

	debugPrint("Loading patch ");
	debugPrintln(_config.patchNumber);
	applyPatch(_config.currentState);	
	displayPatchNumber(_config.patchNumber);

	debugPrintln("Setup MIDI...");
	setupMidi();

	debugPrintln("Setup done !");

	_mode = PLAYING;
	digitalWrite(EDIT_LED_PIN, LOW);
}

void loop()
{
	byte bttn;
	if(input && (bttn = debounceInput()) != LOW)
	{
		//do what we need to do under the current state
		debugPrintln("Interrupted by :");
		debugPrintln(bttn);
		if(_mode == EDITING)
		{
			_currentEditValue ^= bttn;
			applyPatch(_currentEditValue);
		}
	}

	if(edit && (bttn = debounceEdit()) != LOW)
	{
		debugPrintln("Edit pressed");
		swichState();
	}
	
	//handle user ui
	if(_blinkLoopStates) blinkLoopStates();
	if(_blinkEditLed) blinkEditLed();
	if(_blinkDisplay) blinkDisplay();

	MIDI.read();
}
