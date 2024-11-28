#include "User.h"

User::User(std::string _version) : mChatVersion(_version)
{
}

User::~User()
{
	if (host != nullptr)
	{
		delete host;
	}
	if (client != nullptr)
	{
		delete client;
	}
}

//Update function - automatically called once every tick.
void User::Update()
{
	//Host portion
	if (host != nullptr && mServerOpen == true)
	{
		mCommandCalled = false;

		//Check if someone new has joined
		std::shared_ptr<ClientSocket> client = host->Accept();
		if (client)
		{
			//std::cout << "Client Connected" << std::endl;
			mClientList.push_back(std::make_shared<Client>(client));
			
			//int r;
			//int g;
			//int b;
			//do
			//{
			//	r = 1 + rand() % 255;
			//	g = 1 + rand() % 255;
			//	b = 1 + rand() % 255;
			//} while (r == 255 && g == 255 && b == 255);
			//mClientList.back()->SetChatColor(r, g, b);

			//Add username, or check if it conflicts with another user
			std::string username;
			while (mClientList.back()->GetSocket()->Receive(username))
			{
				Command(username, mClientList.size() - 1);
			}

			//Check if new user needs to be removed (invalid username / server is full)
			if (mRemoveUser == true)
			{
				mClientList.pop_back();
			}
			else
			{
				SendCustom("{SYSTEM}: " + username.substr(5) + " has joined the server. Say hi!\n");
				SendToBuffer("{SYSTEM}: " + username.substr(5) + " has joined the server. Say hi!\n");
			}
		}

		//For each client, check if there's new data
		for (size_t ci = 0; ci < mClientList.size(); ++ci)
		{
			std::string message;
			while (mClientList.at(ci)->GetSocket()->Receive(message))
			{
				//std::cout << "Received Message: " << message << std::endl;

				//Check if received message was a command
				if (Command(message, ci) == false)
				{
					//Send message to other users and host's buffer
					message = message.substr(2);
					Send(message, ci);
					SendToBuffer(AddPrefix(ci) + message + "\n");
				}
			}
			
			//Check if client has disconnected
			if (mClientList.at(ci)->GetSocket()->GetClosed())
			{
				//std::cout << "Client Disconnected" << std::endl;
				SendCustom("{SYSTEM}: " + mClientList.at(ci)->GetUsername() + " has left the server. :(\n");
				SendToBuffer("{SYSTEM}: " + mClientList.at(ci)->GetUsername() + " has left the server. :(\n");
				mClientList.erase(mClientList.begin() + ci);
				--ci;
			}
		}
	}
	//Client portion
	else if (client != nullptr)
	{
		//Check if there's a message to be received from the server
		std::string message;
		if (client->Receive(message) == true)
		{
			//Check if received message is a command
			if (Command(message) == false)
			{
				//Send to user's buffer
				SendToBuffer(message);
			}
		}
	}
}

//Initialises the Host part of the class - creates a server and allows incoming connections from other users
void User::InitHost(std::string _username, int _serverSize)
{
	mUsername = _username;

	//If the host socket already exists, delete it and create a new one
	if (host != nullptr)
	{
		delete host;
		host = NULL;
	}
	host = new HostSocket(8080);

	//Set parameters
	mServerSize = _serverSize;
	mIsHost = true;
	mServerOpen = true;

	//Set room code in chat window
	mRoomCode = host->GetRoomCode();
	//std::cout << mRoomCode << std::endl;
}

//Initialises the Client part of the class - attempts to join a server using the address given
void User::InitClient(std::string _username, std::string _target)
{
	mUsername = _username;

	//If the client socket already exists, delete it and create a new one
	if (client != nullptr)
	{
		delete client;
	}
	client = new ClientSocket(_target, 8080, mUsername);
	
	//Set parameters
	mIsHost = false;
	
	//Set room code in chat window
	mRoomCode = _target;
}

//General send function - sends a message to every user except the one specified
void User::Send(std::string _msg, int _userIterator)
{
	//Host portion
	if (host != nullptr)
	{
		//std::cout << "Send " <<  _userIterator << std::endl;

		//If message has a u: prefix, remove it
		if (_msg.substr(0, 2) == "u:" && _userIterator == -1)
		{
			_msg = _msg.substr(2);
		}

		//Add the sender's prefix onto the message
		_msg = AddPrefix(_userIterator) + _msg + "\n";

		//Send it to each client except the sender
		for (size_t si = 0; si < mClientList.size(); ++si)
		{
			if (si != _userIterator)
			{
				//Attempt to send message
				try
				{
					mClientList.at(si)->GetSocket()->Send(_msg);
				}
				//On fail, assume the user has left
				catch (std::runtime_error& e)
				{
					SendCustom("{SYSTEM}: " + mClientList.at(si)->GetUsername() + " has left the server. :(\n");
					SendToBuffer("{SYSTEM}: " + mClientList.at(si)->GetUsername() + " has left the server. :(\n");
					mClientList.erase(mClientList.begin() + si);
					--si;
				}
			}
		}
	}
	//Client portion
	else if (client != nullptr)
	{
		//Attempt to send message
		try
		{
			client->Send(_msg);
		}
		//On fail, assume server has closed
		catch (std::runtime_error& e)
		{
			mServerResponse = DISCONNECT;
		}
		
	}
}

//Whisper send function - sends a message to the targeted user
void User::SendWhisper(std::string _msg, int _userIterator, int _targetUser)
{
	if (host != nullptr)
	{
		//std::cout << "Whisper to " << _targetUser << " from " << _userIterator << std::endl;

		//Add custom wrapper and whisper prefix to message
		_msg = "[" + AddWhisperPrefix(_userIterator, _targetUser) + _msg + "]\n";

		//Send whisper to both users

		if (_userIterator == -1)
		{
			SendToBuffer(_msg);
		}
		else
		{
			mClientList.at(_userIterator)->GetSocket()->Send(_msg);
		}

		if (_targetUser == -1)
		{
			SendToBuffer(_msg);
		}
		else
		{
			mClientList.at(_targetUser)->GetSocket()->Send(_msg);
		}
	}
}

//Warning send function - used by the server to respond to an invalid request
void User::SendWarning(Warnings _warning, int _targetUser)
{
	if (host != nullptr)
	{
		//std::cout << "Warning to " << _targetUser << " with Warning: " << _warning << std::endl;

		//If warning is related to username
		if (_warning == INVALIDUSERNAME)
		{
			//Send warning to user who attempted to call the command
			if (_targetUser == -1)
			{
				SendToBuffer("{SYSTEM}: INVALID USERNAME\n");
			}
			else
			{
				mClientList.at(_targetUser)->GetSocket()->Send("{SYSTEM}: INVALID USERNAME\n");
			}
		}
		//If warning is related to whisper
		else if (_warning == INVALIDWHISPER)
		{
			//Send warning to user who attempted to call the command
			if (_targetUser == -1)
			{
				SendToBuffer("{SYSTEM}: INVALID WHISPER\n");
			}
			else
			{
				mClientList.at(_targetUser)->GetSocket()->Send("{SYSTEM}: INVALID WHISPER\n");
			}
		}
	}
}

//Custom send function - used by the server to respond in a particular way
void User::SendCustom(std::string _msg, int _userIterator)
{
	//If iterator isn't -1, send to target user
	if (_userIterator != -1)
	{
		mClientList.at(_userIterator)->GetSocket()->Send(_msg);
	}
	//Else, send to every user
	else
	{
		for (size_t si = 0; si < mClientList.size(); ++si)
		{
			mClientList.at(si)->GetSocket()->Send(_msg);
		}
	}
}

//Command function - used primarily by the server to respond to requests from the user. Typically involves commands such as changing username, etc.
bool User::Command(std::string _message, int _clientIterator)
{
	//If received message is a command
	if (_message.substr(0, 6) == "u:/cmd")
	{
		//std::cout << "COMMAND RECEIVED" << std::endl;

		//If command is requesting to change username
		if (_message.substr(7, 2) == "cu")
		{
			//std::cout << "CHANGE USERNAME" << std::endl;

			//Host portion
			if (host != nullptr)
			{
				//If user is host, check if their username is valid
				if (_clientIterator == -1)
				{
					if (IsUsernameValid(_message.substr(10)) == true)
					{
						//If valid, announce it and change username
						std::string systemMessage = "{SYSTEM}: " + mUsername + " has changed their username to: " + _message.substr(10) + "\n";
						mUsername = _message.substr(10);
						SendToBuffer(systemMessage);
						SendCustom(systemMessage);
					}
					else
					{
						SendWarning(INVALIDUSERNAME, _clientIterator);
					}
				}
				//Else, check if the specified user's username is valid
				else
				{
					if (IsUsernameValid(_message.substr(10), _clientIterator) == true)
					{
						//If valid, announce it and change username
						std::string systemMessage = "{SYSTEM}: " + mClientList.at(_clientIterator)->GetUsername() + " has changed their username to: " + _message.substr(10) + "\n";
						mClientList.at(_clientIterator)->SetUsername(_message.substr(10));
						SendToBuffer(systemMessage);
						SendCustom(systemMessage);
					}
					else
					{
						SendWarning(INVALIDUSERNAME, _clientIterator);
					}
				}
			}
			//Client portion
			else
			{
				//If username is valid, change locally stored username
				if (IsUsernameValid(_message.substr(10)) == true)
				{
					mUsername = _message.substr(10);
				}
			}
		}
		////Change chat color
		//else if (_message.substr(7, 2) == "cc")
		//{
		//	std::cout << "CHANGE CHAT COLOR" << std::endl;
		//	if (_message.substr(10).length() == 9)
		//	{
		//		int r = atoi(_message.substr(10, 3).c_str());
		//		int g = atoi(_message.substr(13, 3).c_str());
		//		int b = atoi(_message.substr(16, 3).c_str());

		//		if (_clientIterator != -1 && host != nullptr)
		//		{
		//			mClientList.at(_clientIterator)->SetChatColor(r, g, b);
		//		}
		//		else
		//		{
		//			mMessageColor[0] = r, mMessageColor[1] = g, mMessageColor[2] = b;
		//		}
		//	}
		//	else
		//	{
		//		if (_clientIterator != -1 && host != nullptr)
		//		{
		//			SendWarning(INVALIDCOLOR, _clientIterator);
		//		}
		//		else
		//		{
		//			//Send color warning to buffer
		//		}
		//	}
		//}
	}
	//If received message is a whisper
	else if (_message.substr(0, 4) == "u:/w")
	{
		if (host != nullptr)
		{
			//Check if Whisper is valid
			int usernameEnd = _message.substr(5).find(' ');
			if (usernameEnd != std::string::npos)
			{
				std::string targetUsername = _message.substr(5, usernameEnd);

				//std::cout << "Whisper target: " << targetUsername << std::endl;

				//Check if username is a user on the server
				bool isValid{ false };
				int usernameIterator{ 0 };

				//Check if target is host
				if (targetUsername == mUsername)
				{
					isValid = true;
					usernameIterator = -1;
				}
				//Else, iterate through the client list
				else
				{
					for (int x = 0; x < mClientList.size(); x++)
					{
						//std::cout << mClientList.at(x)->GetUsername() << " -> " << targetUsername << std::endl;
						//std::cout << (mClientList.at(x)->GetUsername() == targetUsername) << std::endl;

						//Check if target is user
						///if (mClientList.at(x)->GetUsername() == targetUsername);
						/// I have absolutely NO IDEA WHY, but using the line above ^ as the if statement doesn't work - usernames must be converted to c_str.
						/// An hour and a half lost debugging this silly crap
						if (strcmp(mClientList.at(x)->GetUsername().c_str(), targetUsername.c_str()) == 0)
						{
							//std::cout << "Usernames Match!" << std::endl;
							isValid = true;
							usernameIterator = x;
							break;
						}
					}
				}

				//If username is valid, send the whisper
				if (isValid == true)
				{
					SendWhisper(_message.substr(6 + usernameEnd), _clientIterator, usernameIterator);
				}
				//Else, send a warning to the sender
				else
				{
					SendWarning(INVALIDWHISPER, _clientIterator);
				}
			}
			//Else, send a warning to the sender
			else
			{
				SendWarning(INVALIDWHISPER, _clientIterator);
			}
		}
	}
	//If received message is a client initiating data
	else if (_message.substr(0, 4) == "c:/s")
	{
		if (host != nullptr)
		{
			//Check if the username is valid
			if (IsUsernameValid(_message.substr(5), _clientIterator) == false)
			{
				SendCustom("s:/r q1", _clientIterator);
				mRemoveUser = true;
			}
			//Check if the server is full
			else if (mClientList.size() == mServerSize)
			{
				SendCustom("s:/r q2", _clientIterator);
				mRemoveUser = true;
			}
			//Else, set their username and don't remove them
			else
			{
				mClientList.at(_clientIterator)->SetUsername(_message.substr(5));
				mRemoveUser = false;
			}
		}
	}
	//If received message is a server response
	else if (_message.substr(0, 4) == "s:/r")
	{
		if (_message.substr(5, 1) == "r")
		{
			mServerResponse = REJECT;
		}
		else if (_message.substr(5, 1) == "a")
		{
			mServerResponse = ACCEPT;
		}
		else if (_message.substr(5, 2) == "q1")
		{
			mServerResponse = QUITINVALID;
		}
		else if (_message.substr(5, 2) == "q2")
		{
			mServerResponse = QUITFULL;
		}
	}
	//If received message is a help command
	else if (_message == "u:/help")
	{
		//Host portion
		if (host != nullptr)
		{
			//If the host is initiating the command
			if (_clientIterator == -1)
			{
				const char* helpText = "{SYSTEM}: Commands:\n"
					"\t/cmd cu [USERNAME] - Change your username\n"
					"\t/w [TARGET USERNAME] [MESSAGE] - Private message user\n"
					"\t/help - Query commands\n"
					"\t/about - Returns Application version\n";
				SendToBuffer(helpText);
			}
		}
		//Client portion
		else
		{
			const char* helpText = "{SYSTEM}: Commands:\n"
				"\t/cmd cu [USERNAME] - Change your username\n"
				"\t/w [TARGET USERNAME] [MESSAGE] - Private message user\n"
				"\t/help - Query Commands\n"
				"\t/about s - Returns Server Application Version\n"
				"\t/about c - Returns Client Application Version\n";
			SendToBuffer(helpText);
		}
	}
	//If received message is a query to server version
	else if (_message == "u:/about s")
	{
		if (host != nullptr)
		{
			if (_clientIterator == -1)
			{
				SendToBuffer("{SYSTEM}: Server " + GetChatVersion() + "\n");
			}
			else
			{
				SendCustom("{SYSTEM}: Server " + GetChatVersion() + "\n", _clientIterator);
			}
		}
	}
	//If received message is a query to client version
	else if (_message == "u:/about c")
	{
		//Host portion
		if (host != nullptr)
		{
			if (_clientIterator == -1)
			{
				SendToBuffer("{SYSTEM}: Client " + GetChatVersion() + "\n");
			}
		}
		//Client portion
		else
		{
			SendToBuffer("{SYSTEM}: Client " + GetChatVersion() + "\n");
		}
	}
	else
	{
		return false;
	}

	return true;
}

//Returns if the user is the host
bool User::IsHost()
{
	return mIsHost;
}

//Returns stored username
std::string User::GetUsername()
{
	return mUsername;
}
//Sets username
void User::SetUsername(std::string _username)
{
	mUsername = _username;
}

//Returns received message
std::string User::GetSentMessage()
{
	std::string send = mMessageBuffer;
	mMessageBuffer.clear();
	return send;
}

//Username checker - checks if a username fits the requirements set by the server
bool User::IsUsernameValid(std::string _username, int _clientIterator)
{
	std::string specialCharacters = " !\"#$%&'()*+,./:;<=>?@[\\]^`{|}~";

	bool isValid{ true };

	//Check if username is 10 characters or less
	if (_username.length() > 10)
	{
		isValid = false;
	}
	else
	{
		//For every character, attempt to find a space or the special characters specified above
		for (char c : _username)
		{
			if ((c == ' ') || (specialCharacters.find(c) != std::string::npos))
			{
				isValid = false;
				break;
			}
		}
		//Check if username is unique
		if (IsUsernameUnique(_username, _clientIterator) == false)
		{
			isValid = false;
		}
	}

	return isValid;
}

//Username checker - checks if a username is unique
bool User::IsUsernameUnique(std::string _username, int _clientIterator)
{
	//If the user is the host
	if(_clientIterator == -1)
	{
		//Check if the username conflict with another's
		for (int x = 0; x < mClientList.size(); x++)
		{
			if (_username == mClientList[x]->GetUsername())
			{
				return false;
			}
		}
	}
	else
	{
		//Check if the username conflict with the host's
		if (_username == mUsername)
		{
			return false;
		}
		//Check if the username conflict with another's
		for (int x = 0; x < mClientList.size(); x++)
		{
			if (_clientIterator != x)
			{
				if (_username == mClientList[x]->GetUsername())
				{
					return false;
				}
			}
		}
	}

	return true;
}

//Returns room code
std::string User::GetRoomCode()
{
	return mRoomCode;
}

//Resets variables
void User::Reset()
{
	mServerResponse = WAITING;

	if (host != nullptr)
	{
		mServerOpen = false;

		delete host;
		host = NULL;
	}
	if (client != nullptr)
	{
		client->Close();
	}
}

//Returns server response
int User::GetServerResponse()
{
	return mServerResponse;
}
//Returns server message
std::string User::GetWarningMessage()
{
	return mWarningMessage;
}

//Prefix function, used by the server to prefix the sender's username before the message is sent to other users
std::string User::AddPrefix(int _clientIterator)
{
	std::string prefix;
	if (_clientIterator == -1)
	{
		prefix = mUsername + ": ";
	}
	else
	{
		prefix = mClientList.at(_clientIterator)->GetUsername() + ": ";
	}

	return prefix;
}

//Whisper prefix function, used by the server to custom prefix a whisper before it is sent
std::string User::AddWhisperPrefix(int _clientIterator, int _targetUser)
{
	std::string prefix;

	if (_clientIterator == -1)
	{
		prefix += mUsername + " == ";
	}
	else
	{
		prefix += mClientList.at(_clientIterator)->GetUsername() + " == ";
	}

	if (_targetUser == -1)
	{
		prefix += mUsername + ": ";
	}
	else
	{
		prefix += mClientList.at(_targetUser)->GetUsername() + ": ";
	}

	return prefix;
}

//Function used to send a message to the user's local buffer
void User::SendToBuffer(std::string _message)
{
	mMessageBuffer = _message;
}

//Returns chat version
std::string User::GetChatVersion()
{
	return mChatVersion;
}
