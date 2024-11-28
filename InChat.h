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

/*
  Class used to categorise widgets and logic into one place.

  InChat represents the chat window, where once connected, users can communicate with each other using the attached widgets.
*/
class InChat : public Fl_Double_Window
{
public:
	InChat(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h);

	//Overridden handle function - checks if the '/' key is pressed and if true, gives the input box keyboard focus. 
	//Returns 0 and allows other handlers to handle key events.
	int handle(int event) override;
	
	//Update function - automatically called once every tick when active.
	void Update();

	//Changes the room code label
	void SetRoomCodeLabel(std::string _label);

	//Resets variables
	void Reset();

private:
	//Reference to main window
	MainWindow* mParentWindow;
	//Reference to user
	std::shared_ptr<User> user;

	//Header

	std::unique_ptr<Fl_Centered_Output> mRoomCodeLabel;
	std::unique_ptr<Fl_Box> mTitleBar;

	std::unique_ptr<Fl_Button> mQuitButton;
	//Static reference to Quit() function
	static void StaticQuit(Fl_Widget* _widget, void* _userdata);
	//Closes chat window and returns to menu
	void Quit();

	//Body

	std::unique_ptr<Fl_Text_Buffer> mBuffer;
	std::unique_ptr<Fl_Text_Display> mDisplay;

	//Footer

	std::unique_ptr<Fl_Input> mInput;
	std::unique_ptr<Fl_Button> mSendButton;
	//Static reference to TextInput() function
	static void StaticTextInput(Fl_Widget* _widget, void* _userdata);
	//Grabs data, if any, present in the input box and sends it to the server
	void TextInput();
};

