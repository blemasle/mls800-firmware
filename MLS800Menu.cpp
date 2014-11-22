 #include "MLS800Menu.h"

//------------------------------------------------------
// menu items declaration
//------------------------------------------------------

MenuItem root = {
	"menu",
	0,
	&midi,
	&memClr,
	&midi,
	&menuDisplay,
	0,
	&menuExit,
	0,
	0
};

MenuItem midi = {
	"midi",
	&root,
	&midiR,
	&memClr,
	&dim,
	&menuDisplay,
	0,
	0,
	0,
	0
};

MenuItem midiR = {
	"r   ",
	&midi,
	&midiR_Edit,
	&midiR,
	&midiR,
	&midiRDisplay,
	0,
	0,
	0,
	0
};

MenuItem midiR_Edit = {
	"r.   ",
	&midiR,
	0,
	0,
	0,
	&midiRDisplay,
	&midiRSave,
	&midiRBack,
	&midiRDown,
	&midiRUp
};

MenuItem dim = {
	"dim ",
	&root,
	&dim_Edit,
	&midi,
	&memClr,
	&menuDisplay,
	0,
	0,
	0,
	0
};

MenuItem dim_Edit = {
	"d.   ",
	&dim,
	0,
	0,
	0,
	&dimDisplay,
	&dimSave,
	&dimBack,
	&dimDown,
	&dimUp
};

MenuItem memClr = {
	"clr ",
	&root,
	&memClrYes,
	&dim,
	&midi,
	&menuDisplay,
	0,
	0,
	0,
	0
};

MenuItem memClrYes = {
	"yes ",
	&memClr,
	&memClrd,
	&memClrNo,
	&memClrNo,
	&menuDisplay,
	&factoryReset,
	0,
	0,
	0
};

MenuItem memClrNo = {
	"no  ",
	&memClr,
	0,
	&memClrYes,
	&memClrYes,
	&menuDisplay,
	&factoryResetCancel,
	0,
	0,
	0
};

MenuItem memClrd = {
	"clrd",
	&memClr,
	0,
	0,
	0,
	&factoryResetDoneDisplay,
	0,
	0,
	0,
	0
};

Menu menu = Menu(&root);

//------------------------------------------------------
// menu implementations
//------------------------------------------------------

MENU_ACTION menuDisplay(const char* text)
{
	_inMenu = true;
	_display.display(text, false);

	return MENU_ACTION_NONE;
}

MENU_ACTION menuExit()
{
	_inMenu = false;
	displayPatchNumber(_config.patchNumber);
	return MENU_ACTION_NONE;
}

MENU_ACTION midiRDisplay(const char* text)
{
	char* alteredText = strdup(text);
	printNumber(alteredText, _config.rxChannel);
	_display.display(alteredText, false);
	free(alteredText);

	return MENU_ACTION_NONE;
}

MENU_ACTION midiRBack()
{
	readConfig();
	return MENU_ACTION_BACK;
}

MENU_ACTION midiRSave()
{
	writeConfig();
	HARD_MIDI_PORT.flush();
	HARD_MIDI_PORT.end();
	MIDI.begin(_config.rxChannel);

	return MENU_ACTION_BACK;
}

MENU_ACTION midiRDown()
{
	_config.rxChannel = --_config.rxChannel % 17;
	if (_config.rxChannel < 0) _config.rxChannel += 17;

	return MENU_ACTION_REFRESH;
}

MENU_ACTION midiRUp()
{
	_config.rxChannel = ++_config.rxChannel % 17;

	return MENU_ACTION_REFRESH;
}

MENU_ACTION dimDisplay(const char* text)
{
	char* alteredText = strdup(text);
	printNumber(alteredText, _config.displayDim);
	_display.display(alteredText, false);
	free(alteredText);

	_display.setIntensity(_config.displayDim);
	return MENU_ACTION_NONE;
}

MENU_ACTION dimBack()
{
	readConfig();
	_display.setIntensity(_config.displayDim);
	return MENU_ACTION_BACK;
}

MENU_ACTION dimSave()
{
	writeConfig();

	return MENU_ACTION_BACK;
}

MENU_ACTION dimDown()
{
	_config.displayDim = --_config.displayDim % 16;
	if (_config.displayDim < 0) _config.displayDim += 16;

	return MENU_ACTION_REFRESH;
}

MENU_ACTION dimUp()
{
	_config.displayDim = ++_config.displayDim % 16;

	return MENU_ACTION_REFRESH;
}

MENU_ACTION factoryReset()
{
	uint16_t addr = 0;
	uint16_t i = 0;
	uint8_t dots;
	
	debugPrint("Factory reset... ")
	_display.clear();
	do {
		addr += _storage.erasePage(addr);
		dots = (i++ / 2) % 8;

		if (dots > 3) {
			for (uint8_t digit = 0; digit < dots - 3; digit++) {
				_display.display(digit, BLANK);
			}
		}
		else {
			for (uint8_t digit = 0; digit <= dots; digit++) {
				_display.display(digit, DOT);
			}
		}
	} while (addr < E24LC256_MAXADRESS);
	debugPrintln("Done !");

	resetConfig();

	return MENU_ACTION_SELECT;
}

MENU_ACTION factoryResetCancel()
{
	return MENU_ACTION_BACK;
}

MENU_ACTION factoryResetDoneDisplay(const char* text)
{
	menuDisplay(text);
	delay(CONFIG_CLEARED_TIMEOUT);

	asm volatile ("  jmp 0");

	return MENU_ACTION_NONE;
}