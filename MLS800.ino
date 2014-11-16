#include "MLS800.h"
#include <Wire.h>
#include <MCP23017.h>
#include <AS1115.h>
#include <E24LC256.h>
#include <MIDI.h>

MCP23017 _ui = MCP23017(UI_ADDR);
E24LC256 _storage = E24LC256(STORAGE_ADDR);
AS1115 _display = AS1115(DISPLAY_ADDR);
MCP23017 _loops = MCP23017(LOOP_ADDR);

PatchManager _patchMngr = PatchManager(&_storage);
Config _config;

DeviceState _mode;
byte _editingValue;

byte _currentLoopStates;
byte _currentInputStates;
byte _currentEditBttnState;
byte _currentExitBttnState;

byte _currentEditValue;

long _previousMillis;
long _previousEditLedMillis;
long _previousDisplayMillis;
bool _ledsShuttedOff = false;
bool _displayShuttedOff = true;
bool _blinkLoopStates;
bool _blinkEditLed;
bool _blinkDisplay;
bool _inMenu;

volatile bool input = false;

//===================== PATCHES ======================//

void applyPatch(byte patch)
{
	debugPrint("Loading patch ");
	debugPrintln(_config.patchNumber);
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

void setup()
{
#ifdef _DEBUG
	//arduino leornardo/micro : waiting for serial monitor to be opened
	//in debug mode
	int i = 0;
	Serial.begin(9600);
	while(!Serial) {
		digitalWrite(EDIT_LED_PIN, HIGH);
		delay(500);
		digitalWrite(EDIT_LED_PIN, LOW);
		delay(100);
	}
#endif

	debugPrintln("Setup...");
	Wire.begin();	
	setupStorage();	
	readConfig();	
	setupPatchManager();	
	initUi(_config.displayDim);
	_display.display(_config.version);
		
	setupIo();	
	setupLoops();

	applyPatch(_config.currentState);	
	displayPatchNumber(_config.patchNumber);

	setupMidi();

	debugPrintln("Setup done !");

	_mode = PLAYING;
	digitalWrite(EDIT_LED_PIN, LOW);
}

void loop()
{
	byte bttn;
	if (input && (bttn = readInput()) != LOW)
	{
		debugPrint("Interrupted by :");
		debugPrintln(bttn);
		if (bttn > EXIT_BTTN) {
			if (_mode == EDITING)
			{
				_currentEditValue ^= 1 << (bttn - 9);
				applyPatch(_currentEditValue);
			}
			else if(_inMenu)
			{
				switch(bttn)
				{
				case DOWN_BTTN:
					menu.MenuAction(MENU_ACTION_DOWN);
					break;
				case UP_BTTN:
					menu.MenuAction(MENU_ACTION_UP);
					break;
				}
			}
			else if (_mode == PLAYING) {
				//up and down button to manually switch patch
				switch (bttn)
				{
				case DOWN_BTTN:
					handleProgramChange(0, _config.patchNumber - 1);
					break;
				case UP_BTTN:
					handleProgramChange(0, _config.patchNumber + 1);
					break;
				}
			}
		}
		else {
			switch (bttn) {
			case EDIT_BTTN:
				if (_inMenu)menu.MenuAction(MENU_ACTION_SELECT);
				else swichState();
				break;
			case EXIT_BTTN:
				if (_mode != PLAYING) {
					cancelEditing();
				}
				else if (!_inMenu)menu.MenuAction(MENU_ACTION_SELECT);
				else menu.MenuAction(MENU_ACTION_BACK);
				break;
			}
		}
	}

	//handle user ui
	if(_blinkLoopStates) blinkLoopStates();
	if(_blinkEditLed) blinkEditLed();
	if(_blinkDisplay) blinkDisplay();

//	MIDI.read();

#ifdef _DEBUG
	if(millis() - debugLastMillis > 5000) 
	{
		debugLastMillis = millis();
		debugPrint("free ram : ");
		debugPrintln(freeRam());
		debugPrint("in menu : ");
		debugPrintln(_inMenu);
	}
#endif
}
