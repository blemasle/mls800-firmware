#ifndef _MLS800_h
#define _MLS800_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "MLS800_version.h"
#include "PatchManager.h"

#define CONFIG_ADDR 0
#define CONFIG_SEED "AUDE"

#define UI_ADDR 0x20
#define DISPLAY_ADDR 0x38
#define STORAGE_ADDR 0x50

#define UI_BTN_PORT 1
#define UI_LEDS_PORT 0
#define UI_INT_PIN 1

#define DEBOUNCE_DELAY 100

#define BLINK_DELAY_LOW 250
#define BLINK_DELAY_HIGH 750

#define PATCHES_ADDR E24LC256_PAGESIZE
#define PATCH_COUNT 128
#define CC_COUNT 128

enum DeviceState
{
	PLAYING,
	LEARNING,
	EDITING
};

struct Config
{
	char seed[5];
	char version[6];
	byte rxChannel;
	byte txChannel;
	byte patchNumber;
	byte currentState;
	byte displayDim;
};

#endif