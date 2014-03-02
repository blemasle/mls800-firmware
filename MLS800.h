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
#define CONFIG_SEED "AUDR"

#define UI_ADDR 0x20
#define DISPLAY_ADDR 0x38
#define STORAGE_ADDR 0x50
#define LOOP_ADDR 0x24

#define UI_BTN_PORT 1
#define UI_LEDS_PORT 0
#ifdef __AVR_ATmega32U4__
#define UI_INT PCINT4
#else
#define UI_INT 1
#endif

#ifdef __AVR_ATmega32U4__
#define EDIT_INT INT6
#define EDIT_BTTN_PIN 7
#else
#define EDIT_INT 0
#define EDIT_BTTN_PIN 2
#endif

#define EDIT_LED_PIN 13

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
	char version[7];
	byte rxChannel;
	byte txChannel;
	byte patchNumber;
	byte currentState;
	byte displayDim;
};

#endif