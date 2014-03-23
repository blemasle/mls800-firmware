#ifndef _MLS800_Menu_h
#define _MLS800_Menu_h

#include "Menu.h"

extern Menu menu;
extern MenuItem root;

extern MenuItem midi;
extern MenuItem midiR;
extern MenuItem midiR_Edit;

extern MenuItem dim;
extern MenuItem dim_Edit;

extern MenuItem memClr;
extern MenuItem memClrYes;
extern MenuItem memClrNo;
extern MenuItem memClrd;

MENU_ACTION menuDisplay(char* text);
MENU_ACTION menuExit();

MENU_ACTION midiRDisplay(char* text);
MENU_ACTION midiRBack();
MENU_ACTION midiRSave();
MENU_ACTION midiRDown();
MENU_ACTION midiRUp();

MENU_ACTION dimDisplay(char* text);
MENU_ACTION dimBack();
MENU_ACTION dimSave();
MENU_ACTION dimDown();
MENU_ACTION dimUp();

MENU_ACTION factoryReset();

#endif