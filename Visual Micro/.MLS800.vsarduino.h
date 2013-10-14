#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Nano w/ ATmega328
#define __AVR_ATmega328P__
#define 
#define _DEBUG 1
#define ARDUINO 105
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 16000000L
#define _MCP23017_INTERRUPT_SUPPORT_
#define _MCP23017_INTERRUPT_DEBOUNCE_
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

void startSerial();
void endSerial();
void uiInterrupt();
void editInterrupt();
byte debounceInput();
byte debounceEdit();
void setupDisplay(SAA1064_DIM dim);
void setupUi();
void displayLoopStates(byte state);
void blinkLoopStates();
void stopBlinkLoopStates();
void setupStorage();
void setupPatchManager();
void writeConfig();
void resetConfig();
void readConfig();
void startEditing();
void swichState();
void printNumber(char * s, byte nb);
void displayPatchNumber(byte nb);
//
//

#include "C:\Apps\Development\IDE\Arduino\hardware\arduino\variants\eightanaloginputs\pins_arduino.h" 
#include "C:\Apps\Development\IDE\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "E:\LTouristDocuments\Development\Arduino\MLS800\MLS800.ino"
#include "E:\LTouristDocuments\Development\Arduino\MLS800\MLS800.h"
#include "E:\LTouristDocuments\Development\Arduino\MLS800\MLS800_version.h"
#include "E:\LTouristDocuments\Development\Arduino\MLS800\PatchManager.cpp"
#include "E:\LTouristDocuments\Development\Arduino\MLS800\PatchManager.h"
#endif
