#include "Menu.h"
#include "MainWindow.h"

Menu::Menu(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h)
	: Fl_Window(_x, _y, _w, _h),
	mParentWindow(_parent),
	mServerButton(100, 100, 200, 100, "SERVER")
{
	user = _user;

	mServerButton.callback(StaticButtonListener, (void*)this);

	end();
}

void Menu::Update()
{
	
}

void Menu::ChangeState(int _state)
{
	mParentWindow->ChangeState(_state);
}

//Button listeners
//Server button
void Menu::StaticButtonListener(Fl_Widget* w, void* _userdata)
{
	Menu* buttonFunction = (Menu*)_userdata;
	buttonFunction->ButtonListener();
}
void Menu::ButtonListener()
{
	user->InitHost();
	ChangeState(1);
}