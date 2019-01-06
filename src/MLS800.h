#ifndef _MLS800_h
#define _MLS800_h

//------------------------------------------------------
// includes
//------------------------------------------------------

#include <Arduino.h>

// libraries includes
#include <Wire.h>
#include <MCP23017.h>
#include <AS1115.h>
#include <E24.h>
#include <MIDI.h>
#include <midi_UsbTransport.h>

// internal includes
#include "MLS800_version.h"
#include "PatchManager.h"
#include "MLS800Menu.h"
#include "Debug.h"

//------------------------------------------------------
// defines
//------------------------------------------------------

// external chips addresses
#define DISPLAY_ADDR 0x03
#define STORAGE_ADDR 0x50
#define LOOP_ADDR 0x24

// config base address and seed to check integrity
#define CONFIG_ADDR 0
#define CONFIG_SEED "AUDE"
#define CONFIG_CLEARED_TIMEOUT 1500

// config storage configuration
#define STORAGE_SIZE E24Size_t::E24_256K
#define STORAGE_MAX_ADDR E24_MAX_ADDRESS(STORAGE_SIZE)
#define PATCHES_ADDR E24_PAGE_SIZE(STORAGE_SIZE)
#define PATCH_COUNT 128
#define CC_COUNT 128

// user interface system definitions
#define UI_PIN 8
#define EDIT_LED_PIN 13

#define BLINK_DELAY_LOW 250
#define BLINK_DELAY_HIGH 750
#define VERSION_DISPLAY_DELAY 1500

// user input buttons
#define EDIT_BTTN 3
#define EXIT_BTTN 4

#define DOWN_BTTN 15
#define UP_BTTN 16

//midi
#define HARD_MIDI_PORT USE_SERIAL_PORT

//------------------------------------------------------
// macros
//------------------------------------------------------

// clearing and setting bits macros
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//------------------------------------------------------
// typedefs, enums, structs etc
//------------------------------------------------------

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
	uint8_t patchNumber;
	uint8_t currentState;
	uint8_t displayDim;
};

//------------------------------------------------------
// global variables
//------------------------------------------------------

extern bool _setupDone;

extern uint8_t _currentLoopStates;
extern uint8_t _currentEditValue;

extern long _startedMillis;
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

extern volatile bool _inputInterrupted;

extern E24 _storage;
extern AS1115 _display;
extern MCP23017 _loops;
extern PatchManager _patchMngr;


MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

static const unsigned sUsbTransportBufferSize = 16;
typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;
UsbTransport sUsbTransport;
MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, MIDIUSB);

//------------------------------------------------------
// prototypes
//------------------------------------------------------

void writeConfig();
void resetConfig();
void readConfig();

void displayPatchNumber(uint8_t nb);
void printNumber(char* s, uint8_t nb);

void setupStorage();
void setupPatchManager();
void setupUi(uint8_t dim);
void setupInterrupts();
void setupIo();
void setupLoops();
void setupMidi();
void setupDone();

void handleProgramChange(uint8_t channel, uint8_t number);
void handleControlChange(uint8_t channel, uint8_t number, uint8_t value);
void readMIDI();

void applyPatch(uint8_t patch);
uint8_t loadPatch(uint8_t patchNumber);

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
void displayLoopStates(uint8_t state);
void blinkLoopStates();
void stopBlinkLoopStates();

uint8_t readInput();
void applyLoopStates(uint8_t state);
uint8_t reverseByte(uint8_t v);

#endif