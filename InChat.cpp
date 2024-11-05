#include "InChat.h"

InChat::InChat(std::shared_ptr<User> _user, int _x, int _y, int _w, int _h)
	: Fl_Window(_x, _y, _w, _h), Object(this),
	mBuffer(),
	mDisplay(100, 100, 200, 200),
	mInput(100, 400, 75, 75, "Text: ")
{
	user = _user;

	mDisplay.buffer(mBuffer);
	
	mInput.when(FL_WHEN_ENTER_KEY);
	mInput.callback(StaticTextInput, (void*)this);

	end();

	hide();
}

void InChat::Update()
{

}

void InChat::StaticTextInput(Fl_Widget* _widget, void* _userdata)
{
	InChat* InputFunction = (InChat*)_userdata;
	InputFunction->TextInput();
}

void InChat::TextInput()
{
	mBuffer.append("\n", 2);
	mBuffer.append(mInput.value(), mInput.size());
	mInput.static_value("");
}