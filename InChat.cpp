#include "InChat.h"
#include "MainWindow.h"

InChat::InChat(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h)
	: Fl_Window(_x, _y, _w, _h),
	mParentWindow(_parent),
	mBuffer(),
	mDisplay(100, 100, 400, 200),
	mInput(100, 400, 75, 25, "Text: ")
{
	user = _user;

	mDisplay.buffer(mBuffer);
	
	mInput.when(FL_WHEN_ENTER_KEY);
	mInput.maximum_size(80);
	mInput.callback(StaticTextInput, (void*)this);

	end();
	hide();
}

void InChat::Update()
{
	std::string newMessage = user->GetSentMessage();
	if (newMessage.empty() == false)
	{
		mBuffer.append(newMessage.c_str(), newMessage.length());
	}

	if (user->GetServerResponse() == QUIT)
	{
		mParentWindow->ChangeState(0);
		fl_alert("Rejected from server:\n Username Invalid or Not Unique\n Usernames cannot have spaces or special characters.");
	}
}

void InChat::Reset()
{
	if (mBuffer.length() != 0)
	{
		mBuffer.remove(0, mBuffer.length());
	}
}

void InChat::StaticTextInput(Fl_Widget* _widget, void* _userdata)
{
	InChat* InputFunction = (InChat*)_userdata;
	InputFunction->TextInput();
}

void InChat::TextInput()
{
	std::string text;

	text.append(mInput.value());

	bool isCommand = user->Command("u:" + text);
	bool sendMessage{ false };

	if (isCommand == false)
	{
		std::string bufferText = user->GetUsername() + ": " + text;
		bufferText.append("\n");
		mBuffer.append(bufferText.c_str(), bufferText.size());

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

	mInput.static_value("");
}