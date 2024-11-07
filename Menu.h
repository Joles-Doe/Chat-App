#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <FL/Fl_Button.H>

#include <memory>
#include "User.h"

class MainWindow;

class Menu : public Fl_Window
{
public:
	Menu(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h);

	void Update();

	void ChangeState(int _state);

private:
	MainWindow* mParentWindow;

	std::shared_ptr<User> user;

	//Buttons and listeners
	//Server button
	Fl_Button mServerButton;
	static void StaticButtonListener(Fl_Widget* w, void* _userdata);
	void ButtonListener();
};