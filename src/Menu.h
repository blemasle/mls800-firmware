#pragma once

enum MENU_ACTION {
	MENU_ACTION_NONE,
	MENU_ACTION_UP,
	MENU_ACTION_DOWN,
	MENU_ACTION_SELECT,
	MENU_ACTION_BACK,
	MENU_ACTION_REFRESH
};

struct MenuItem {
	const char* text;
	MenuItem* parent;
	MenuItem* child;
	MenuItem* prev;
	MenuItem* next;
	MENU_ACTION (*display)(const char* text);
	MENU_ACTION (*select)();
	MENU_ACTION (*back)();
	MENU_ACTION (*down)();
	MENU_ACTION (*up)();
};

class Menu {
private :
	MenuItem* _root;
	MenuItem* _current;
public:
	Menu(MenuItem* root);
	void MenuAction(MENU_ACTION action);
};