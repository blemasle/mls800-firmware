#include "MLS800.h"

void startLearning()
{
	_blinkEditLed = true;
	_blinkDisplay = true;
	_currentEditValue = _config.currentState;
}

void endLearning()
{
	displayPatchNumber(_config.patchNumber);
}

void startEditing()
{
	_blinkEditLed = false;
	stopBlinkDisplay();
	_currentEditValue = _config.currentState;
	digitalWrite(EDIT_LED_PIN, HIGH);
}

void endEditing()
{
	_config.currentState = _currentEditValue;
	writeConfig();
	_patchMngr.save(_config.patchNumber, _config.currentState);
	digitalWrite(EDIT_LED_PIN, LOW);
}

void cancelEditing()
{
	_blinkEditLed = false;
	stopBlinkDisplay();
	digitalWrite(EDIT_LED_PIN, LOW);
	_mode = PLAYING;
	loadPatch(_config.patchNumber);
}

void swichState() {
	switch (_mode)
	{
	case PLAYING:
		_mode = LEARNING;
		startLearning();
		break;
	case LEARNING:
		endLearning();
		_mode = EDITING;
		startEditing();
		break;
	case EDITING:
		endEditing();
		_mode = PLAYING;
		break;
	}
	debugPrintln("Device State changed :");
	debugPrintln(_mode == PLAYING ? "PLAYING" : _mode == LEARNING ? "LEARNING" : "EDITING");
}