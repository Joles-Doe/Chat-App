#include "Menu.h"
#include "MainWindow.h"

Menu::Menu(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h)
	: Fl_Window(_x, _y, _w, _h),
	mParentWindow(_parent)
{
	user = _user;

	mMenuTabs = std::make_unique<Fl_Tabs>(0, 50, _w, _h, "LABEL");
	{
		mServerTab = std::make_unique<Fl_Group>(10, 70, _w - 20, _h - 20, "Server");
		{
			mServerUsernameInput = std::make_unique<Fl_Input>(100, 400, 75, 25, "Username: ");
			mServerUsernameInput->maximum_size(10);

			mServerButton = std::make_unique<Fl_Button>(100, 100, 200, 100, "SERVER");
			mServerButton->callback(StaticServerButton, (void*)this);
		}
		mServerTab->end();

		mClientTab = std::make_unique<Fl_Group>(10, 70, _w - 20, _h - 20, "Client");
		{
			// >:(
			mClientUsernameInput = std::make_unique<Fl_Input>(
				mServerUsernameInput->x(),
				mServerUsernameInput->y(),
				mServerUsernameInput->w(),
				mServerUsernameInput->h(),
				mServerUsernameInput->label());
			mClientUsernameInput->maximum_size(10);

			mClientButton = std::make_unique<Fl_Button>(100, 100, 200, 100, "CLIENT");
			mClientButton->callback(StaticClientButton, (void*)this);
		}
		mClientTab->end();
	}
	mMenuTabs->end();
	end();
}

Menu::~Menu()
{
}

void Menu::Update()
{
}

void Menu::ChangeState(int _state)
{
	mParentWindow->ChangeState(_state);
}

//Inputs and Buttons listeners

//Server button
void Menu::StaticServerButton(Fl_Widget* w, void* _userdata)
{
	Menu* buttonFunction = (Menu*)_userdata;
	buttonFunction->ServerButton();
}
void Menu::ServerButton()
{
	try
	{
		user->InitHost(mServerUsernameInput->value());
	}
	catch (const std::runtime_error& e)
	{
		fl_alert(e.what());
		return;
	}
	ChangeState(1);
}

//Client button
void Menu::StaticClientButton(Fl_Widget* w, void* _userdata)
{
	Menu* buttonFunction = (Menu*)_userdata;
	buttonFunction->ClientButton();
}
void Menu::ClientButton()
{
	try
	{
		user->InitClient(mClientUsernameInput->value());
	}
	catch (const std::runtime_error& e)
	{
		fl_alert(e.what());
		return;
	}
	ChangeState(1);
}
