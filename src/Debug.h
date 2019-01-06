#ifndef _Debug_h
#define _Debug_h

#include <Arduino.h>

#ifdef _DEBUG

extern long debugLastMillis;

int freeRam();
void startSerial();
void endSerial();
void debug();

#define debugPrintln(msg) startSerial();Serial.println(msg);endSerial();
#define debugPrint(msg) startSerial();Serial.print(msg);endSerial();
#define debugPrintlnBase(msg, base) startSerial();Serial.println(msg, base);endSerial();
#define debugPrintBase(msg, base) startSerial();Serial.print(msg, base);endSerial();

#else

#define debugPrintln(msg)
#define debugPrint(msg)
#define debugPrintlnBase(msg, base)
#define debugPrintBase(msg, base)

#endif

#endif