#include "Menu.h"

#include "MainWindow.h"

Menu::Menu(std::shared_ptr<User> _user, void* _parent, int _x, int _y, int _w, int _h)
	: Fl_Window(_x, _y, _w, _h), Object(this),
	parent(_parent),
	mServerButton(&mServerButton, this, 100, 100, 200, 200)
{
	user = _user;
	end();
}

void Menu::Update()
{
	
}

void Menu::ChangeParentState(int _state)
{
	MainWindow* windowParent = (MainWindow*)parent;
	windowParent->ChangeState(_state);
}

void Menu::ParentInitHost()
{
	MainWindow* windowParent = (MainWindow*)parent;
	windowParent->InitHost();
}