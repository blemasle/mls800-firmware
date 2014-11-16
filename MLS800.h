#ifndef _MLS800_h
#define _MLS800_h

//------------------------------------------------------
// Includes
//------------------------------------------------------

#include <Arduino.h>

// libraries includes
#include <Wire.h>
#include <MCP23017.h>
#include <AS1115.h>
#include <E24LC256.h>
#include <MIDI.h>

// internal includes
#include "MLS800_version.h"
#include "PatchManager.h"
#include "MLS800Menu.h"
#include "Debug.h"

//------------------------------------------------------
// Defines
//------------------------------------------------------

// external chips addresses
#define UI_ADDR 0x20
#define DISPLAY_ADDR 0x03
#define STORAGE_ADDR 0x50
#define LOOP_ADDR 0x24

// config base address and seed to check integrity
#define CONFIG_ADDR 0
#define CONFIG_SEED "AUDR"

#define UI_BTN_PORT 1
#define UI_LEDS_PORT 0

#define UI_PIN 8
#define UI_INT PCINT4
#define UI_INT_PIN PINB4

#define EDIT_BTTN 3
#define EXIT_BTTN 4

#define DOWN_BTTN 15
#define UP_BTTN 16

#define EDIT_LED_PIN 13

#define DEBOUNCE_DELAY 100

#define BLINK_DELAY_LOW 250
#define BLINK_DELAY_HIGH 750

#define PATCHES_ADDR E24LC256_PAGESIZE
#define PATCH_COUNT 128
#define CC_COUNT 128

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

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
	int8_t rxChannel;
	int8_t txChannel;
	byte patchNumber;
	byte currentState;
	byte displayDim;
};

extern byte _currentLoopStates;
extern byte _currentInputStates;
extern byte _currentEditBttnState;
extern byte _currentExitBttnState;

extern byte _currentEditValue;
extern long _previousMillis;
extern long _previousEditLedMillis;
extern long _previousDisplayMillis;
extern bool _ledsShuttedOff;
extern bool _displayShuttedOff;
extern bool _blinkLoopStates;
extern bool _blinkEditLed;
extern bool _blinkDisplay;
extern bool _inMenu;

extern Config _config;
extern DeviceState _mode;

extern volatile bool input;

extern MCP23017 _ui;
extern E24LC256 _storage;
extern AS1115 _display;
extern MCP23017 _loops;
extern PatchManager _patchMngr;

void writeConfig();
void resetConfig();
void readConfig();

void displayPatchNumber(byte nb);
void printNumber(char * s, byte nb);

void setupStorage();
void setupPatchManager();
void initUi(byte dim);
void setupInterrupts();
void setupIo();
void setupLoops();
void setupMidi();

void handleProgramChange(byte channel, byte number);
void handleControlChange(byte channel, byte number, byte value);

void applyPatch(byte patch);
byte loadPatch(byte patchNumber);

void startLearning();
void endLearning();
void startEditing();
void endEditing();
void cancelEditing();
void swichState();


void blinkEditLed();
void blinkDisplay();
void stopBlinkDisplay();
void clearDisplay();
void displayLoopStates(byte state);
void blinkLoopStates();
void stopBlinkLoopStates();

byte readInput();
void applyLoopStates(byte state);

#endif