#include "MLS800.h"

void handleProgramChange(uint8_t channel, uint8_t number)
{
	debugPrint("Program Change : ");
	debugPrintln(number);
	if (_mode == PLAYING)
	{
		_config.patchNumber = number;
		loadPatch(_config.patchNumber);
	}
	else if (_mode == LEARNING)
	{
		_config.patchNumber = number;
		swichState();
	}
}

void handleControlChange(uint8_t channel, uint8_t number, uint8_t value)
{
	debugPrint("Control Change : ");
	debugPrint(number);
	debugPrint(" ");
	debugPrintln(value);
}

void readMIDI()
{
	//reads MIDI commands from serial port
	MIDI.read();

	//reads MIDI commands from USB
	while (MIDIUSB.available() > 0) {
		MIDIEvent e;
		e = MIDIUSB.read();

		debugPrint("received MIDI from USB : ");
		debugPrintlnBase(e.type, HEX);

		if (e.type == 0xB) {
			handleControlChange(_config.rxChannel, e.m2, e.m3);
		}
		else if (e.type == 0xC) {
			handleProgramChange(_config.rxChannel, e.m2);
		}
	}
}