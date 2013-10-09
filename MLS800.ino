#include "MLS800.h"
#include <Wire.h>
#include <MCP23017.h>
#include <SAA1064.h>
#include <E24LC256.h>
#include <MIDI.h>

MCP23017 _ui = MCP23017(UI_ADDR);
E24LC256 _storage = E24LC256(STORAGE_ADDR);
SAA1064 _display = SAA1064(DISPLAY_ADDR);
Config _config;

byte _currentLoopStates;
byte _currentInputStates;

long _previousMillis;
bool _ledsShuttedOff;
bool _blink;

volatile bool input = false;

#ifdef _DEBUG
#define printDebug(msg) startSerial();Serial.println(msg);Serial.end();
void startSerial()
{
	endSerial();
	Serial.begin(115200);
}

void endSerial()
{
	Serial.end();
}
#else
#define printDebug
#endif

//interrupt handler for user input
void uiInterrupt()
{
	input = true;
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
	attachInterrupt(UI_INT_PIN, uiInterrupt, CHANGE);
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

//storage configuration
void setupStorage()
{
	//nothing to do yet
}

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

void setup()
{
	Wire.begin();
	printDebug("Setup storage...");
	setupStorage();
	printDebug("Reading configuration...");
	readConfig();
	printDebug("Setup display...");
	setupDisplay((SAA1064_DIM)_config.displayDim);
	_display.display(_config.version);
	
	printDebug("Setup user interface...");
	setupUi();
	displayLoopStates(_config.lastState);
	printDebug("Setup done !");
#if _DEBUG
	_ui.debug();
#endif

}

void loop()
{
	//handle user input first
	byte bttn;
	if(input && (bttn = debounceInput()) != LOW)
	{
		//do what we need to do under the current state
		printDebug("Interrupted by :");
		printDebug(bttn);
	}

	if(_blink) blinkLoopStates();
}
