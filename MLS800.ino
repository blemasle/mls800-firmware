#include "MLS800.h"
#include <Wire.h>
#include <MCP23017.h>
#include <SAA1064.h>
#include <E24LC256.h>
#include <MIDI.h>

MCP23017 _ui = MCP23017(UI_ADDR);
E24LC256 _storage = E24LC256(STORAGE_ADDR);
SAA1064 _display = SAA1064(DISPLAY_ADDR);

//interrupt handler for user input
void userInput()
{
}

//display configuration
void setupDisplay()
{
	_display.init(DYNAMIC, MID);
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

void setup()
{
	Wire.begin();

	setupDisplay();
	setupUi();
	setupStorage();

  /* add setup code here */

}

void loop()
{

  /* add main program code here */

}
