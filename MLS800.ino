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


void startSerial()
{
	endSerial();
	Serial.begin(115200);
}

void endSerial()
{
	Serial.end();
}


//interrupt handler for user input
void userInput()
{
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

	//port a : input
	_ui.portMode(0, 0);
	//port b : output
	_ui.portMode(0, 0xFF);

	_ui.interruptMode(OR);
	_ui.interrupt(1, FALLING);

	_ui.clearInterrupts();
	attachInterrupt(1, userInput, FALLING);
}

void setupStorage()
{
	//nothing to do yet
}

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

void setup()
{
	Wire.begin();
	
	setupStorage();
	readConfig();
	setupDisplay((SAA1064_DIM)_config.displayDim);
	_display.display(_config.version);

	setupUi();

	
}

void loop()
{

  /* add main program code here */

}
