#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef _MLS800_h
#define _MLS800_h

#define UI_ADDR 0x20
#define DISPLAY_ADDR 0x20
#define STORAGE_ADDR 0x20

struct Config
{
	char seed[5];
	char version[5];
	byte rxChannel;
	byte txChannel;
	byte lastPath;
	byte lastState;
	byte displayDim;
};

#endif