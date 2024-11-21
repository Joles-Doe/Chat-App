#include "InChat.h"

InChat::InChat(std::shared_ptr<User> _user, int _x, int _y, int _w, int _h)
	: Fl_Window(_x, _y, _w, _h),
	mBuffer(),
	mDisplay(100, 100, 400, 200),
	mInput(100, 400, 75, 25, "Text: ")
{
	user = _user;

	mDisplay.buffer(mBuffer);
	
	mInput.when(FL_WHEN_ENTER_KEY);
	mInput.maximum_size(115);
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

	user->Send(text);

	if (user->Command(text) == false)
	{
		text = user->GetUsername() + ": " + text;
		text.append("\n");
		mBuffer.append(text.c_str(), text.size());
	}

	mInput.static_value("");
}