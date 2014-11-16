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

void displayLoopStates(byte state)
{
	_display.display(5, state);
	//_ui.writePort(UI_LEDS_PORT, state);
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