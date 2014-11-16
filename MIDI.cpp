#include "MLS800.h"

void handleProgramChange(byte channel, byte number)
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

void handleControlChange(byte channel, byte number, byte value)
{
	debugPrint("Control Change : ");
	debugPrint(number);
	debugPrint(" ");
	debugPrintln(value);
}