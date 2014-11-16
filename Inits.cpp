#include "MLS800.h"

//storage configuration
void setupStorage()
{
	debugPrintln("Setup storage...");
	//nothing to do yet
}

//patch manager configuration
void setupPatchManager()
{
	debugPrintln("Setup patch manager...");

	_patchMngr.init(PATCHES_ADDR, PATCH_COUNT, CC_COUNT);
}


//display configuration
void setupUi(byte dim)
{
	debugPrintln("Setup display...");

	_display.init(5, dim);
}

//device specific interrupts configuration
void setupInterrupts()
{
	//INT6 is not supported by attachInterrupt function, so going into manual mode
	//disable interrupt on INT6
	cbi(EIMSK, INT6);

	//interrupt INT6 on FALLING
	cbi(EICRB, ISC60);
	sbi(EICRB, ISC61);

	//clear interruput flag for INT6
	sbi(EIFR, INTF6);

	//re enable INT6
	sbi(EIMSK, INT6);

	//reset any pending interrupt on the external chip
	_display.read();
}

//io configuration
void setupIo()
{
	debugPrintln("Setup user interface...");

	pinMode(UI_PIN, INPUT);
	digitalWrite(UI_PIN, HIGH);

	pinMode(EDIT_LED_PIN, OUTPUT);
}

void setupLoops()
{
	debugPrintln("Setup loops...");

	_loops.init();
	//all ports to output
	//port A : ON port
	_loops.portMode(0, 0x00);
	//port B : OFF port
	_loops.portMode(1, 0x00);
}

void setupMidi()
{
	debugPrint("Setup MIDI on channel ");
	debugPrint(_config.rxChannel);
	debugPrintln();

	MIDI.begin(_config.rxChannel);
	MIDI.setHandleControlChange(handleControlChange);
	MIDI.setHandleProgramChange(handleProgramChange);
}

void setupDone()
{
	setupInterrupts();

	debugPrintln("Setup done !");
	displayPatchNumber(_config.patchNumber);	
	_mode = PLAYING;

	_setupDone = true;
}