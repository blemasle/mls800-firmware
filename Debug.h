#ifndef _Debug_h
#define _Debug_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifdef _DEBUG

int freeRam();
void startSerial();
void endSerial();
extern long debugLastMillis;
#define debugPrintln(msg) startSerial();Serial.println(msg);endSerial();
#define debugPrint(msg) startSerial();Serial.print(msg);endSerial();
#define debugPrintlnBase(msg, base) startSerial();Serial.println(msg, base);endSerial();
#define debugPrintBase(msg, base) startSerial();Serial.print(msg, base);endSerial();

#else

#define debugPrintln
#define debugPrint
#define debugPrintlnBase
#define debugPrintBase

#endif

#endif