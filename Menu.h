#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Clock.H>
#include <FL/Fl_Wizard.H>
#include <FL/Fl_Output.h>
#include "Fl_Centered_Output.h"

#include <memory>
#include "User.h"

class MainWindow;

/*
  Class used to categorise widgets and logic into one place.

  Menu represents the menu window, where users can choose to create or join a server.
*/
class Menu : public Fl_Double_Window
{
public:
	Menu(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h);
	~Menu();

	void Update();

private:
	//Reference to main window
	MainWindow* mParentWindow;
	//Reference to user
	std::shared_ptr<User> user;

	//Generates a random username using acceptable characters
	std::string GenerateUsername();

	//Header

	std::unique_ptr<Fl_Box> mTitle;

	//Body

	std::unique_ptr<Fl_Box> mInputLabel;
	std::unique_ptr<Fl_Input> mUsernameInput;

	std::unique_ptr<Fl_Clock> mClock;

	std::unique_ptr<Fl_Button> mServerButton;
	//Static reference to ServerButton() function
	static void StaticServerButton(Fl_Widget* w, void* _userdata);
	//Changes wizard tab present in the footer to the server settings tab
	void ServerButton();

	std::unique_ptr<Fl_Button> mClientButton;
	//Static reference to ClientButton() function
	static void StaticClientButton(Fl_Widget* w, void* _userdata);
	//Changes wizard tab present in the footer to the client settings tab
	void ClientButton();
	
	//Footer

	std::unique_ptr<Fl_Wizard> mContent;

	std::unique_ptr<Fl_Group> mServerSettings;
	std::unique_ptr<Fl_Group> mClientSettings;
	
	std::unique_ptr<Fl_Box> mServerSizeLabel;
	std::unique_ptr<Fl_Slider> mServerSize;
	//Static reference to UpdateSlider() function
	static void StaticUpdateSlider(Fl_Widget* w, void* _userdata);
	//Grabs the value in the slider and puts it in the slider output box
	void UpdateSlider();

	std::unique_ptr<Fl_Centered_Output> mSizeOutput;

	std::unique_ptr<Fl_Box> mClientTargetLabel;
	std::unique_ptr<Fl_Input> mClientTarget;

	
	std::unique_ptr<Fl_Button> mStartButton;
	//Static reference to StartButton() function
	static void StaticStartButton(Fl_Widget* w, void* _userdata);
	//Performs username check, connects / hosts dependent on active tab, and changes window state
	void StartButton();
};