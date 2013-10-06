#ifndef _MLS800_h
#define _MLS800_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "MLS800_version.h"

#define CONFIG_ADDR 0
#define CONFIG_SEED "AUDE"

#define UI_ADDR 0x20
#define DISPLAY_ADDR 0x38
#define STORAGE_ADDR 0x50

struct Config
{
	char seed[5];
	char version[6];
	byte rxChannel;
	byte txChannel;
	byte lastPatch;
	byte lastState;
	byte displayDim;
};

#endif