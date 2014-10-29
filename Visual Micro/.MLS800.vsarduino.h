/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Micro (arcore), Platform=avr, Package=arcore
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __AVR_ATmega32u4__
#define __AVR_ATmega32U4__
#define USB_VID 0x2341
#define USB_PID 0x8037
#define USB_MANUFACTURER 
#define USB_PRODUCT "\"Arduino Micro\""
#define ARDUINO 155
#define ARDUINO_MAIN
#define __AVR__
#define __avr__
#define F_CPU 16000000L
#define _MCP23017_INTERRUPT_SUPPORT_
#define _DEBUG
#define _AS1115_DIAGNOSTICS_
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
void exitInterrupt();
byte debounceInput();
byte debounceEdit();
byte debounceExit();
MENU_ACTION menuDisplay(const char* text);
MENU_ACTION menuExit();
MENU_ACTION midiRDisplay(const char* text);
MENU_ACTION midiRBack();
MENU_ACTION midiRSave();
MENU_ACTION midiRDown();
MENU_ACTION midiRUp();
MENU_ACTION dimDisplay(const char* text);
MENU_ACTION dimBack();
MENU_ACTION dimSave();
MENU_ACTION dimDown();
MENU_ACTION dimUp();
MENU_ACTION factoryReset();
void setupStorage();
void setupPatchManager();
void setupDisplay(byte dim);
void setupInterrupts();
void setupIo();
void setupLoops();
void setupMidi();
void handleProgramChange(byte channel, byte number);
void handleControlChange(byte channel, byte number, byte value);
void blinkEditLed();
void blinkDisplay();
void stopBlinkDisplay();
void displayLoopStates(byte state);
void blinkLoopStates();
void stopBlinkLoopStates();
void applyLoopStates(byte state);
void writeConfig();
void resetConfig();
void readConfig();
void applyPatch(byte patch);
byte loadPatch(byte patchNumber);
void startLearning();
void endLearning();
void startEditing();
void endEditing();
void swichState();
void printNumber(char * s, byte nb);
void displayPatchNumber(byte nb);
//
int freeRam ();
//

#include "C:\Apps\Development\IDE\Arduino\hardware\arcore\avr\cores\arcore\arduino.h"
#include "C:\Apps\Development\IDE\Arduino\hardware\arcore\avr\variants\micro\pins_arduino.h" 
#include "E:\Working Copies\Arduino\mls800\MLS800.ino"
#include "E:\Working Copies\Arduino\mls800\MLS800.h"
#include "E:\Working Copies\Arduino\mls800\MLS800Menu.cpp"
#include "E:\Working Copies\Arduino\mls800\MLS800Menu.h"
#include "E:\Working Copies\Arduino\mls800\MLS800_version.h"
#include "E:\Working Copies\Arduino\mls800\Menu.cpp"
#include "E:\Working Copies\Arduino\mls800\Menu.h"
#include "E:\Working Copies\Arduino\mls800\PatchManager.cpp"
#include "E:\Working Copies\Arduino\mls800\PatchManager.h"
#endif
