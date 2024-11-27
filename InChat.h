#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>

#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include "Fl_Centered_Output.h"
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#include <string>
#include <memory>
#include "User.h"

class MainWindow;

class InChat : public Fl_Double_Window
{
public:
	InChat(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h);

	void Update();

	void SetRoomCodeLabel(std::string _label);
	void Reset();

private:
	MainWindow* mParentWindow;
	std::shared_ptr<User> user;

	std::unique_ptr<Fl_Output> mRoomCodeLabel;
	std::unique_ptr<Fl_Box> mTitleBar;
	std::unique_ptr<Fl_Button> mQuitButton;
	static void StaticQuit(Fl_Widget* _widget, void* _userdata);
	void Quit();

	std::unique_ptr<Fl_Text_Buffer> mBuffer;
	std::unique_ptr<Fl_Text_Display> mDisplay;

	//Buttons and listeners
	//Input button
	std::unique_ptr<Fl_Input> mInput;
	static void StaticTextInput(Fl_Widget* _widget, void* _userdata);
	void TextInput();
};

