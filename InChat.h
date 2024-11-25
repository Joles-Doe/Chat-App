#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>

#include <string>
#include <memory>
#include "User.h"

class MainWindow;

class InChat : public Fl_Window
{
public:
	InChat(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h);

	void Update();

	void Reset();

private:
	MainWindow* mParentWindow;
	std::shared_ptr<User> user;

	Fl_Text_Buffer mBuffer;
	Fl_Text_Display mDisplay;

	//Buttons and listeners
	//Input button
	Fl_Input mInput;
	static void StaticTextInput(Fl_Widget* _widget, void* _userdata);
	void TextInput();
};

