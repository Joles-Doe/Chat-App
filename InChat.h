#pragma once

#include "Object.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>

#include <string>
#include <memory>
#include "User.h"

class InChat : public Fl_Window, Object
{
public:
	InChat(std::shared_ptr<User> _user, int _x, int _y, int _w, int _h);

	void Update();

private:
	std::shared_ptr<User> user;

	Fl_Text_Buffer mBuffer;
	Fl_Text_Display mDisplay;
	Fl_Input mInput;

	static void StaticTextInput(Fl_Widget* _widget, void* _userdata);
	void TextInput();
};
