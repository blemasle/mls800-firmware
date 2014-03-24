#include "Menu.h"
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifdef _DEBUG
#define debugPrintln(msg) startSerial2();Serial.println(msg);endSerial2();
#define debugPrint(msg) startSerial2();Serial.print(msg);endSerial2();
void startSerial2()
{
	Serial.end();
	Serial.begin(115200);
}

void endSerial2()
{
	Serial.flush();
	Serial.end();
}
#endif

Menu::Menu(MenuItem* root)
{
	_root = root;
	_current = _root;
}

void Menu::MenuAction(MENU_ACTION action)
{
	if(!_current || action == MENU_ACTION_NONE) return;

	debugPrint("MenuAction ");
	debugPrintln(action);

	MENU_ACTION result = MENU_ACTION_NONE;
	//requested an action on an item, callback then determine what to do next
	switch(action)
	{
	case MENU_ACTION_UP:
		if(_current->up) result = _current->up();
		else result = action;
		break;
	case MENU_ACTION_DOWN:
		if(_current->down) result = _current->down();
		else result = action;
		break;
	case MENU_ACTION_SELECT:
		if(_current->select) result = _current->select();
		else result = action;
		break;
	case MENU_ACTION_BACK:
		if(_current->back) result = _current->back();
		else result = action;
		break;
	case MENU_ACTION_REFRESH:
		if(_current->display) result = _current->display(_current->text);
		else result = MENU_ACTION_NONE;
		break;
	}

	debugPrint("MenuAction, switch action done. ");
	debugPrint(_current->text);
	debugPrint(", ");
	debugPrintln(result);

	//changing the selected item
	switch(result)
	{
	case MENU_ACTION_UP:
		_current = _current->prev;
		result = MENU_ACTION_REFRESH;
		break;
	case MENU_ACTION_DOWN:
		_current = _current->next;
		result = MENU_ACTION_REFRESH;
		break;
	case MENU_ACTION_SELECT:
		_current = _current->child;
		result = MENU_ACTION_REFRESH;
		break;
	case MENU_ACTION_BACK:
		_current = _current->parent;
		result = MENU_ACTION_REFRESH;
		break;
	}

	MenuAction(result);
}