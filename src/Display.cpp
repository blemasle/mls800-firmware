#include "MLS800.h"

void blinkEditLed()
{
	long currentMillis = millis();
	if (currentMillis - _previousEditLedMillis > BLINK_DELAY_HIGH) {
		digitalWrite(EDIT_LED_PIN, !digitalRead(EDIT_LED_PIN));
		_previousEditLedMillis = currentMillis;
	}
}

void blinkDisplay()
{
	long currentMillis = millis();
	if (currentMillis - _previousDisplayMillis > BLINK_DELAY_HIGH) {
		if (_displayShuttedOff) displayPatchNumber(_config.patchNumber);
		else clearDisplay();

		_displayShuttedOff = !_displayShuttedOff;
		_previousDisplayMillis = currentMillis;
	}
}

void stopBlinkDisplay()
{
	_blinkDisplay = false;
	displayPatchNumber(_config.patchNumber);
	_displayShuttedOff = true;
}

void clearDisplay() {
	_display.clear();
	displayLoopStates(_config.currentState);
}

void displayLoopStates(uint8_t state)
{
#ifdef REV_A
	/* pcb rev A has some errors that we need to compensate :
	*  bttn DP is last instead of first : change the state to display so it is transparent for outer code
	*  DP which is the MSB in the keyscan register become the LSB for the program
	*  DP A B C D E F G is displayed as A B C D E F G DP
	*/
	bool last = state & 0x01;
	state >>= 1;
	if (last) state |= 0x80;
#endif
	_display.display(4, state);
}

void blinkLoopStates()
{
	long currentMillis = millis();
	short delay = _ledsShuttedOff ? BLINK_DELAY_LOW : BLINK_DELAY_HIGH;
	if (currentMillis - _previousMillis > delay) {
		if (_ledsShuttedOff) displayLoopStates(_currentLoopStates);
		else displayLoopStates(0);

		_ledsShuttedOff = !_ledsShuttedOff;
		_previousMillis = currentMillis;
	}
}

void stopBlinkLoopStates()
{
	_blinkLoopStates = false;
	displayLoopStates(_currentLoopStates);
	_ledsShuttedOff = false;
}

void printNumber(char* s, uint8_t nb)
{
	uint8_t pos = nb > 99 ? 3 : nb > 9 ? 2 : 1;
	itoa(nb, s + strlen(s) - pos, 10);
}

void displayPatchNumber(uint8_t nb)
{
	char * str = new char[5];
	strcpy(str, "P   ");
	printNumber(str, nb);
	_display.display(str, false);
}