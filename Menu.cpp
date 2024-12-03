#include "Menu.h"
#include "MainWindow.h"

Menu::Menu(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h)
	: Fl_Double_Window(_x, _y, _w, _h),
	mParentWindow(_parent)
{
	user = _user;

	//Header
	mTitle = std::make_unique<Fl_Box>(0, 0, _w, 125, "Chat App");
	mTitle->labelsize(40);

	//Body
	mInputLabel = std::make_unique<Fl_Box>(25, 140, _w / 2, 75, "Username");
	mInputLabel->labelsize(26);

	mUsernameInput = std::make_unique<Fl_Input>(mInputLabel->w() / 2 - 75, 220, 200, 40);
	mUsernameInput->textsize(20);
	mUsernameInput->maximum_size(10);

	mClock = std::make_unique<Fl_Clock>((_w / 2) + 50, 140, 300, 300);

	mServerButton = std::make_unique<Fl_Button>(25, 300, mInputLabel->w() / 2 - 25, 100, "Host");
	mServerButton->labelsize(20);
	mServerButton->callback(StaticServerButton, (void*)this);

	mClientButton = std::make_unique<Fl_Button>(mInputLabel->w() / 2 + 50, 300, mInputLabel->w() / 2 - 25, 100, "Join");
	mClientButton->labelsize(20);
	mClientButton->callback(StaticClientButton, (void*)this);

	//Footer
	mContent = std::make_unique<Fl_Wizard>(0, 480, _w, _h - 480);
	{
		mServerSettings = std::make_unique<Fl_Group>(0, 480, _w, _h - 480);
		{
			mServerSizeLabel = std::make_unique<Fl_Box>(75, 550, 300, 60, "Server Size");
			mServerSizeLabel->labelsize(26);

			mServerSize = std::make_unique<Fl_Slider>(75, 610, 300, 50);
			mServerSize->value(2);
			mServerSize->bounds(2, 15);
			mServerSize->type(FL_HORIZONTAL);
			mServerSize->labelsize(20);
			mServerSize->callback(StaticUpdateSlider, (void*)this);
			mServerSize->when(FL_WHEN_CHANGED);

			mSizeOutput = std::make_unique<Fl_Centered_Output>(60 + mServerSize->w() / 2, 660, 30, 30);
			mSizeOutput->color(FL_BACKGROUND_COLOR);
			mSizeOutput->textsize(20);
			mSizeOutput->value("2");

		}
		mServerSettings->end();

		mClientSettings = std::make_unique<Fl_Group>(0, 480, _w, _h - 480);
		{
			mClientTargetLabel = std::make_unique<Fl_Box>(75, 550, 300, 60, "Server ID");
			mClientTargetLabel->labelsize(26);

			mClientTarget = std::make_unique<Fl_Input>(75, 610, 300, 40);
			mClientTarget->textsize(20);
		}
		mClientSettings->end();
	}
	mContent->end();

	mStartButton = std::make_unique<Fl_Button>(((_w / 2) + 50) + mClock->w() / 6, 550, 200, 200, "Start");
	mStartButton->labelsize(28);
	mStartButton->callback(StaticStartButton, (void*)this);

	end();
}

Menu::~Menu()
{
}

void Menu::Update()
{
}

//Generates a random username using acceptable characters
std::string Menu::GenerateUsername()
{
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-";
	std::string username;

	for (int x = 0; x < (3 + rand() % 10); x++)
	{
		username += alphabet[rand() % (sizeof(alphabet) / sizeof(alphabet[0]))];
	}
	return username;
}

//Static reference to ServerButton() function
void Menu::StaticServerButton(Fl_Widget* w, void* _userdata)
{
	Menu* buttonFunction = (Menu*)_userdata;
	buttonFunction->ServerButton();
}
//Changes wizard tab present in the footer to the server settings tab
void Menu::ServerButton()
{
	mContent->prev();
}

//Static reference to ClientButton() function
void Menu::StaticClientButton(Fl_Widget* w, void* _userdata)
{
	Menu* buttonFunction = (Menu*)_userdata;
	buttonFunction->ClientButton();
}
//Changes wizard tab present in the footer to the client settings tab
void Menu::ClientButton()
{
	mContent->next();
}

//Static reference to UpdateSlider() function
void Menu::StaticUpdateSlider(Fl_Widget* w, void* _userdata)
{
	Menu* listenerFunction = (Menu*)_userdata;
	listenerFunction->UpdateSlider();
}
//Grabs the value in the slider and puts it in the slider output box
void Menu::UpdateSlider()
{
	char buffer[3]{ 0 };
	sprintf_s(buffer, "%d", (int)mServerSize->value());
	mSizeOutput->value(buffer);
}

//Static reference to StartButton() function
void Menu::StaticStartButton(Fl_Widget* w, void* _userdata)
{
	Menu* listenerFunction = (Menu*)_userdata;
	listenerFunction->StartButton();
}
//Performs username check, connects / hosts dependent on active tab, and changes window state
void Menu::StartButton()
{
	std::string username;
	//If the user hasn't entered a username, generate a random one
	if (mUsernameInput->size() == 0)
	{
		username = GenerateUsername();
	}
	//Else, take the username the user has entered
	else
	{
		username = mUsernameInput->value();
	}
	
	//Check if the generated or entered username is unique and doesn't use invalid characters
	if (user->IsUsernameValid(username) == false)
	{
		fl_alert("Invalid username! Please ensure your username contains no special characters.");
		return;
	}

	//Initialize either host or client, dependent on the active settings tab
	try
	{
		if (mContent->value() == mServerSettings.get())
		{
			user->InitHost(username, mServerSize->value());
		}
		else if (mContent->value() == mClientSettings.get())
		{
			user->InitClient(username, mClientTarget->value());
		}
	}
	catch (const std::runtime_error& e)
	{
		fl_alert(e.what());
		return;
	}

	//If no errors occur, change to the chat window.
	mParentWindow->ChangeState(1);
}
