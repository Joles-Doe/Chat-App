#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#include <memory>
#include "User.h"

class MainWindow;

class Menu : public Fl_Window
{
public:
	Menu(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h);
	~Menu();

	void Update();

	void ChangeState(int _state);

private:
	MainWindow* mParentWindow;
	std::shared_ptr<User> user;

	std::unique_ptr<Fl_Tabs> mMenuTabs;

	std::unique_ptr<Fl_Group> mServerTab;
	std::unique_ptr<Fl_Group> mClientTab;

	//Inputs, Buttons and listeners
	//Username Input 
	std::unique_ptr<Fl_Input> mServerUsernameInput;
	std::unique_ptr<Fl_Input> mClientUsernameInput;

	//Server button
	std::unique_ptr<Fl_Button> mServerButton;
	static void StaticServerButton(Fl_Widget* w, void* _userdata);
	void ServerButton();

	//Client button
	std::unique_ptr<Fl_Button> mClientButton;
	static void StaticClientButton(Fl_Widget* w, void* _userdata);
	void ClientButton();
};