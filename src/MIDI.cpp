#include "MLS800.h"

UsbTransport sUsbTransport;
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, UsbMIDI);

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
	UsbMIDI.read();
}