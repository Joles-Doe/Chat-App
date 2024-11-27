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
#include <FL/Fl_Light_Button.H>

#include <memory>
#include "User.h"

class MainWindow;

class Menu : public Fl_Double_Window
{
public:
	Menu(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h);
	~Menu();

	void Update();

	void ChangeState(int _state);

private:
	MainWindow* mParentWindow;
	std::shared_ptr<User> user;

	std::string GenerateUsername();

	//Server button
	std::unique_ptr<Fl_Button> mServerButton;
	static void StaticServerButton(Fl_Widget* w, void* _userdata);
	void ServerButton();

	//Client button
	std::unique_ptr<Fl_Button> mClientButton;
	static void StaticClientButton(Fl_Widget* w, void* _userdata);
	void ClientButton();


	std::unique_ptr<Fl_Box> mTitle;

	std::unique_ptr<Fl_Clock> mClock;

	std::unique_ptr<Fl_Box> mInputLabel;
	std::unique_ptr<Fl_Input> mUsernameInput;

	std::unique_ptr<Fl_Wizard> mContent;

	std::unique_ptr<Fl_Group> mServerSettings;
	std::unique_ptr<Fl_Group> mClientSettings;

	static void StaticUpdateSlider(Fl_Widget* w, void* _userdata);
	void UpdateSlider();
	std::unique_ptr<Fl_Box> mServerSizeLabel;
	std::unique_ptr<Fl_Slider> mServerSize;
	std::unique_ptr<Fl_Output> mSizeOutput;

	std::unique_ptr<Fl_Box> mClientTargetLabel;
	std::unique_ptr<Fl_Input> mClientTarget;

	static void StaticStartButton(Fl_Widget* w, void* _userdata);
	void StartButton();
	std::unique_ptr<Fl_Button> mStartButton;
};