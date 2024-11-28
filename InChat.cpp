#include "InChat.h"
#include "MainWindow.h"

InChat::InChat(std::shared_ptr<User> _user, MainWindow* _parent, int _x, int _y, int _w, int _h)
	: Fl_Double_Window(_x, _y, _w, _h),
	mParentWindow(_parent)
{
	user = _user;

	//Header
	mRoomCodeLabel = std::make_unique<Fl_Centered_Output>(50, 0, 75, 100);
	mRoomCodeLabel->color(FL_WHITE);
	mRoomCodeLabel->textsize(30);

	//mTitleBar = std::make_unique<Fl_Box>(100, 0, _w - 250, 100);

	mQuitButton = std::make_unique<Fl_Button>(_w - 150, 0, 150, 100, "QUIT");
	mQuitButton->labelsize(25);
	mQuitButton->callback(StaticQuit, (void*)this);

	//Body
	mBuffer = std::make_unique<Fl_Text_Buffer>();

	mDisplay = std::make_unique<Fl_Text_Display>(0, 100, _w, _h - 200);
	mDisplay->buffer(*mBuffer);
	mDisplay->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 10);
	mDisplay->textsize(20);

	//Footer
	mInput = std::make_unique<Fl_Input>(0, _h - 100, _w - 150, 100);
	mInput->when(FL_WHEN_ENTER_KEY);
	mInput->maximum_size(80);
	mInput->callback(StaticTextInput, (void*)this);
	mInput->textsize(20);

	mSendButton = std::make_unique<Fl_Button>(_w - 150, _h - 100, 150, 100, "SEND");
	mSendButton->labelsize(25);
	mSendButton->callback(StaticTextInput, (void*)this);

	end();

	//Hide the window, not used by default
	hide();
}

//Overridden handle function - checks if the '/' key is pressed and if true, gives the input box keyboard focus. 
int InChat::handle(int event)
{
	switch (event) 
	{
	case FL_KEYBOARD: 
	{
		if (Fl::event_key() == '/') 
		{
			mInput->take_focus();
			return 1;
		}
		break;
	}
	default:
		break;
	}
	return Fl_Window::handle(event);
}

//Update function - automatically called once every tick when active.
void InChat::Update()
{
	//Check if the user has sent a message and needs to be downloaded to the buffer
	std::string newMessage = user->GetSentMessage();
	if (newMessage.empty() == false)
	{
		mBuffer->append(newMessage.c_str(), newMessage.length());
	}

	//Check if a server response requires the user to quit
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
	else if (user->GetServerResponse() == DISCONNECT)
	{
		mParentWindow->ChangeState(0);
		fl_alert("Disconnected from server:\n Server closed.");
	}
}

//Changes the room code label
void InChat::SetRoomCodeLabel(std::string _label)
{
	mRoomCodeLabel->value(_label.c_str());
}

//Resets variables
void InChat::Reset()
{
	mBuffer->remove(0, mBuffer->length());
	mBuffer->append("{SYSTEM}: CHATS ARE NOT PRIVATE - DO NOT SHARE SENSITIVE INFORMATION\n", 69);
}

//Static reference to Quit() function
void InChat::StaticQuit(Fl_Widget* _widget, void* _userdata)
{
	InChat* ButtonFunction = (InChat*)_userdata;
	ButtonFunction->Quit();
}
//Closes chat window and returns to menu
void InChat::Quit()
{
	mParentWindow->ChangeState(0);
}

//Static reference to TextInput() function
void InChat::StaticTextInput(Fl_Widget* _widget, void* _userdata)
{
	InChat* InputFunction = (InChat*)_userdata;
	InputFunction->TextInput();
}
//Grabs data, if any, present in the input box and sends it to the server
void InChat::TextInput()
{
	std::string text = mInput->value();
	if (text.empty() == false)
	{
		//Check if the user has called a command
		bool isCommand = user->Command("u:" + text);
		bool sendMessage{ false };

		//If the message isn't a command
		if (isCommand == false && (text != "/help"))
		{
			//Append the message to the user's buffer
			std::string bufferText = user->GetUsername() + ": " + text;
			bufferText.append("\n");
			mBuffer->append(bufferText.c_str(), bufferText.size());

			sendMessage = true;
		}
		//Else, if the message is a command and the user isn't the host, send the message to the server
		else if (isCommand == true && user->IsHost() == false)
		{
			sendMessage = true;
		}

		//If a message needs to be sent to the server
		if (sendMessage == true)
		{
			//Client messages are prefixed with "u:" in order to prevent accidental admin abuse
			if (user->IsHost() == false)
			{
				text = "u:" + text;
			}	
			user->Send(text);
		}
		mInput->static_value("");
	}	
}