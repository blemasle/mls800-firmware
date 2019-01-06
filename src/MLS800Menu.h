#ifndef _MLS800_Menu_h
#define _MLS800_Menu_h

#include "MLS800.h"
#include "Menu.h"

extern Menu menu;
extern MenuItem root;

extern MenuItem midiRoot;
extern MenuItem midiR;
extern MenuItem midiR_Edit;

extern MenuItem dim;
extern MenuItem dim_Edit;

extern MenuItem memClr;
extern MenuItem memClrYes;
extern MenuItem memClrNo;
extern MenuItem memClrd;

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
MENU_ACTION factoryResetCancel();
MENU_ACTION factoryResetDoneDisplay(const char* text);

#endif