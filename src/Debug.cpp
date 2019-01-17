#include "MLS800.h"

long debugLastMillis = millis();

void startSerial()
{
	Serial.end();
	Serial.begin(115200);
}

void endSerial()
{
	Serial.flush();
	Serial.end();
}

int freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void debug() {
	if (millis() - debugLastMillis > 5000)
	{
		debugLastMillis = millis();
		debugPrint("free ram : ");
		debugPrintln(freeRam());
		debugPrint("in menu : ");
		debugPrintln(_inMenu);
	}
}