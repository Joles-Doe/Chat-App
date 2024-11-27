#include "InChat.h"
#include "MainWindow.h"

InChat::InChat(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h)
	: Fl_Double_Window(_x, _y, _w, _h),
	mParentWindow(_parent)
{
	user = _user;

	mRoomCodeLabel = std::make_unique<Fl_Output>(50, 0, 75, 100);
	mRoomCodeLabel->color(FL_WHITE);
	mRoomCodeLabel->textsize(30);
	//mTitleBar = std::make_unique<Fl_Box>(100, 0, _w - 250, 100);
	mQuitButton = std::make_unique<Fl_Button>(_w - 150, 0, 150, 100, "QUIT");
	mQuitButton->labelsize(25);
	mQuitButton->callback(StaticQuit, (void*)this);

	mBuffer = std::make_unique<Fl_Text_Buffer>();

	mDisplay = std::make_unique<Fl_Text_Display>(0, 100, _w, _h - 200);
	mDisplay->buffer(*mBuffer);
	mDisplay->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 10);
	mDisplay->textsize(20);

	mInput = std::make_unique<Fl_Input>(0, _h - 100, _w - 100, 100);
	mInput->when(FL_WHEN_ENTER_KEY);
	mInput->maximum_size(80);
	mInput->callback(StaticTextInput, (void*)this);
	mInput->textsize(20);

	end();
	hide();
}

void InChat::Update()
{
	std::string newMessage = user->GetSentMessage();
	if (newMessage.empty() == false)
	{
		mBuffer->append(newMessage.c_str(), newMessage.length());
	}

	if (user->GetServerResponse() == QUITINVALID)
	{
		mParentWindow->ChangeState(0);
		fl_alert("Rejected from server:\n Username Invalid or Not Unique\n Usernames cannot have spaces or special characters.");
	}
	else if (user->GetServerResponse() == QUITFULL)
	{
		mParentWindow->ChangeState(0);
		fl_alert("Rejected from server:\n Server is full.");
	}
}

void InChat::SetRoomCodeLabel(std::string _label)
{
	mRoomCodeLabel->value(_label.c_str());
}

void InChat::Reset()
{
	if (mBuffer->length() != 0)
	{
		mBuffer->remove(0, mBuffer->length());
	}
}

void InChat::StaticQuit(Fl_Widget* _widget, void* _userdata)
{
	InChat* ButtonFunction = (InChat*)_userdata;
	ButtonFunction->Quit();
}
void InChat::Quit()
{
	mParentWindow->ChangeState(0);
}

void InChat::StaticTextInput(Fl_Widget* _widget, void* _userdata)
{
	InChat* InputFunction = (InChat*)_userdata;
	InputFunction->TextInput();
}
void InChat::TextInput()
{
	std::string text;
	text.append(mInput->value());

	bool isCommand = user->Command("u:" + text);
	bool sendMessage{ false };

	if (isCommand == false)
	{
		std::string bufferText = user->GetUsername() + ": " + text;
		bufferText.append("\n");
		mBuffer->append(bufferText.c_str(), bufferText.size());

		sendMessage = true;
	}
	else if (isCommand == true && user->IsHost() == false)
	{
		sendMessage = true;
	}

	if (sendMessage == true)
	{
		if (user->IsHost() == false)
		{
			text = "u:" + text;
		}
		//User messages are prefixed with "u:" in order to prevent accidental admin abuse
		user->Send(text);
	}

	mInput->static_value("");
}