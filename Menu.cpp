#include "Menu.h"
#include "MainWindow.h"

Menu::Menu(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h)
	: Fl_Double_Window(_x, _y, _w, _h),
	mParentWindow(_parent)
{
	user = _user;

	mTitle = std::make_unique<Fl_Box>(0, 0, _w, 125, "FLTK CHAT APPLICATION");
	mTitle->labelsize(40);

	mClock = std::make_unique<Fl_Clock>((_w / 2) + 50, 140, 300, 300);

	mInputLabel = std::make_unique<Fl_Box>(25, 140, _w / 2, 75, "USERNAME");
	mInputLabel->labelsize(26);

	mUsernameInput = std::make_unique<Fl_Input>(mInputLabel->w() / 2 - 75, 220, 200, 40);
	mUsernameInput->textsize(20);
	mUsernameInput->maximum_size(10);

	mServerButton = std::make_unique<Fl_Button>(25, 300, mInputLabel->w() / 2 - 25, 100, "HOST");
	mServerButton->labelsize(20);
	mServerButton->callback(StaticServerButton, (void*)this);

	mClientButton = std::make_unique<Fl_Button>(mInputLabel->w() / 2 + 50, 300, mInputLabel->w() / 2 - 25, 100, "CLIENT");
	mClientButton->labelsize(20);
	mClientButton->callback(StaticClientButton, (void*)this);

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

			mSizeOutput = std::make_unique<Fl_Output>(60 + mServerSize->w() / 2, 660, 30, 30);
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

	mStartButton = std::make_unique<Fl_Button>(((_w / 2) + 50) + mClock->w() / 6, 550, 200, 200, "START");
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

//Server button
void Menu::StaticServerButton(Fl_Widget* w, void* _userdata)
{
	Menu* buttonFunction = (Menu*)_userdata;
	buttonFunction->ServerButton();
}
void Menu::ServerButton()
{
	mContent->prev();
}

//Client button
void Menu::StaticClientButton(Fl_Widget* w, void* _userdata)
{
	Menu* buttonFunction = (Menu*)_userdata;
	buttonFunction->ClientButton();
}
void Menu::ClientButton()
{
	mContent->next();
}

//Slider update
void Menu::StaticUpdateSlider(Fl_Widget* w, void* _userdata)
{
	Menu* listenerFunction = (Menu*)_userdata;
	listenerFunction->UpdateSlider();
}
void Menu::UpdateSlider()
{
	char buffer[3]{ 0 };
	sprintf_s(buffer, "%d", (int)mServerSize->value());
	mSizeOutput->value(buffer);
}

//Start button
void Menu::StaticStartButton(Fl_Widget* w, void* _userdata)
{
	Menu* listenerFunction = (Menu*)_userdata;
	listenerFunction->StartButton();
}
void Menu::StartButton()
{
	std::string username;
	if (mUsernameInput->size() == 0)
	{
		username = GenerateUsername();
	}
	else
	{
		username = mUsernameInput->value();
	}

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
	mParentWindow->ChangeState(1);
}
