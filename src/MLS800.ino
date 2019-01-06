#include "MLS800.h"
#include <Wire.h>
#include <MCP23017.h>
#include <AS1115.h>
#include <E24LC256.h>
#include <MIDI.h>

E24LC256 _storage = E24LC256(STORAGE_ADDR);
AS1115 _display = AS1115(DISPLAY_ADDR);
MCP23017 _loops = MCP23017(LOOP_ADDR);

PatchManager _patchMngr = PatchManager(&_storage);
Config _config;

DeviceState _mode;

bool _setupDone = false;

uint8_t _currentLoopStates;
uint8_t _currentEditValue;

long _startedMillis;
long _previousMillis;
long _previousEditLedMillis;
long _previousDisplayMillis;
bool _ledsShuttedOff = false;
bool _displayShuttedOff = true;
bool _blinkLoopStates;
bool _blinkEditLed;
bool _blinkDisplay;
bool _inMenu;

volatile bool _inputInterrupted = false;

//===================== PATCHES ======================//

void applyPatch(uint8_t patch)
{
	debugPrint("Loading patch ");
	debugPrintln(_config.patchNumber);
	displayLoopStates(patch);
	applyLoopStates(patch);
}

uint8_t loadPatch(uint8_t patchNumber)
{
	uint8_t patch;
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
	Serial.begin(9600);
	while(!Serial) {
		digitalWrite(EDIT_LED_PIN, HIGH);
		delay(500);
		digitalWrite(EDIT_LED_PIN, LOW);
		delay(100);
	}
#endif

	_startedMillis = millis();
	debugPrintln("Setup...");
	Wire.begin();	
	setupStorage();	
	readConfig();		
	setupUi(_config.displayDim);
	_display.display(_config.version);
		
	setupIo();	
	setupLoops();

	setupPatchManager();
	applyPatch(_config.currentState);	

	setupMidi();
}

void loop()
{
	uint8_t bttn;
	if (_inputInterrupted && (bttn = readInput()) != LOW)
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

	if (!_setupDone && millis() - _startedMillis > VERSION_DISPLAY_DELAY) {
		setupDone();
	}

	readMIDI();
#ifdef _DEBUG
	debug();
#endif
}
