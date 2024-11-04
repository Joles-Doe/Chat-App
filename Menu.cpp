#include "Menu.h"

Menu::Menu(std::shared_ptr<User> _user, int _x, int _y, int _w, int _h) 
	: Fl_Group(_x, _y, _w, _h),
	mServerButton(&mServerButton, this, 100, 100, 200, 200)
{
	user = _user;
	end();
}

void Menu::Update()
{

}