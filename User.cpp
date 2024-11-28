#include "User.h"

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

void User::Update()
{
	if (host != nullptr && mServerOpen == true)
	{
		mCommandCalled = false;

		//Check if someone new has joined
		std::shared_ptr<ClientSocket> client = host->Accept();
		if (client)
		{
			std::cout << "Client Connected" << std::endl;
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
		}

		//For each client, check if there's new data
		for (size_t ci = 0; ci < mClientList.size(); ++ci)
		{
			std::string message;

			while (mClientList.at(ci)->GetSocket()->Receive(message))
			{
				std::cout << "Received Message: " << message << std::endl;

				if (Command(message, ci) == false)
				{
					message = message.substr(2);
					Send(message, ci);
					SendToBuffer(AddPrefix(ci) + message + "\n");
				}
			}
			
			//Check if client has disconnected
			if (mClientList.at(ci)->GetSocket()->GetClosed())
			{
				std::cout << "Client Disconnected" << std::endl;
				mClientList.erase(mClientList.begin() + ci);
				--ci;
			}
		}
	}
	else if (client != nullptr)
	{
		std::string message;
		if (client->Receive(message) == true)
		{
			if (Command(message) == false)
			{
				SendToBuffer(message);
			}
		}
	}
}

void User::InitHost(std::string _username, int _serverSize)
{
	mUsername = _username;

	if (host != nullptr)
	{
		delete host;
		host = NULL;
	}
	host = new HostSocket(8080);
	mServerSize = _serverSize;
	mRoomCode = host->GetRoomCode();
	std::cout << mRoomCode << std::endl;
	mIsHost = true;
	mServerOpen = true;
}

void User::InitClient(std::string _username, std::string _target)
{
	mUsername = _username;

	if (client != nullptr)
	{
		delete client;
	}
	mIsHost = false;
	client = new ClientSocket(_target, 8080, mUsername);
	mRoomCode = _target;
}

void User::Send(std::string _msg, int _userIterator)
{
	if (host != nullptr)
	{
		std::cout << "Send " <<  _userIterator << std::endl;

		if (_msg.substr(0, 2) == "u:" && _userIterator == -1)
		{
			_msg = _msg.substr(2);
		}

		_msg = AddPrefix(_userIterator) + _msg + "\n";

		for (size_t si = 0; si < mClientList.size(); ++si)
		{
			if (si != _userIterator)
			{
				try
				{
					mClientList.at(si)->GetSocket()->Send(_msg);
				}
				catch (std::runtime_error& e)
				{
					mClientList.erase(mClientList.begin() + si);
					--si;
				}
			}
		}
	}
	else if (client != nullptr)
	{
		try
		{
			client->Send(_msg);
		}
		catch (std::runtime_error& e)
		{
			mServerResponse = DISCONNECT;
		}
		
	}
}

void User::SendWhisper(std::string _msg, int _userIterator, int _targetUser)
{
	if (host != nullptr)
	{
		std::cout << "Whisper to " << _targetUser << " from " << _userIterator << std::endl;

		_msg = "[" + AddPrefix(_userIterator) + _msg + "]\n";

		//If message is being sent to host
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

void User::SendWarning(Warnings _warning, int _targetUser)
{
	if (host != nullptr)
	{
		std::cout << "Warning to " << _targetUser << " with Warning: " << _warning << std::endl;

		//username
		if (_warning == INVALIDUSERNAME)
		{
			if (_targetUser == -1)
			{
				SendToBuffer("{SYSTEM}: INVALID USERNAME\n");
			}
			else
			{
				mClientList.at(_targetUser)->GetSocket()->Send("{SYSTEM}: INVALID USERNAME\n");
			}
		}
		//whisper
		else if (_warning == INVALIDWHISPER)
		{
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

void User::SendCustom(std::string _msg, int _userIterator)
{
	if (_userIterator != -1)
	{
		mClientList.at(_userIterator)->GetSocket()->Send(_msg);
	}
	else
	{
		for (size_t si = 0; si < mClientList.size(); ++si)
		{
			mClientList.at(si)->GetSocket()->Send(_msg);
		}
	}
}

bool User::IsHost()
{
	return mIsHost;
}

std::string User::GetUsername()
{
	return mUsername;
}

void User::SetUsername(std::string _username)
{
	mUsername = _username;
}

std::string User::GetSentMessage()
{
	std::string send = mMessageBuffer;
	mMessageBuffer.clear();
	return send;
}

int User::GetSentMessageColor(int _colorIndex)
{
	int color{ 0 };
	if (_colorIndex > 0 && _colorIndex < 4)
	{
		color = mMessageColor[_colorIndex];
	}

	return color;
}

bool User::IsUsernameValid(std::string _username, int _clientIterator)
{
	std::string specialCharacters = " !\"#$%&'()*+,./:;<=>?@[\\]^`{|}~";

	bool isValid{ true };

	for (char c : _username)
	{
		if ((c == ' ') || (specialCharacters.find(c) != std::string::npos))
		{
			isValid = false;
			break;
		}
	}
	if (IsUsernameUnique(_username, _clientIterator) == false)
	{
		isValid = false;
	}

	return isValid;
}

bool User::IsUsernameUnique(std::string _username, int _clientIterator)
{
	if(_clientIterator == -1)
	{
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
		if (_username == mUsername)
		{
			return false;
		}
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

std::string User::GetRoomCode()
{
	return mRoomCode;
}

bool User::Command(std::string _message, int _clientIterator)
{
	//If received message is a command
	if (_message.substr(0, 6) == "u:/cmd")
	{
		std::cout << "COMMAND RECEIVED" << std::endl;

		//Change username
		if (_message.substr(7, 2) == "cu")
		{
			std::cout << "CHANGE USERNAME" << std::endl;

			if (host != nullptr)
			{
				if (_clientIterator == -1)
				{
					if (IsUsernameValid(_message.substr(10)) == true)
					{
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
				else
				{
					if (IsUsernameValid(_message.substr(10)) == true)
					{
						std::string systemMessage = "{SYSTEM}: " + mClientList.at(_clientIterator)->GetUsername() + " has changed their username to: " + _message.substr(10) + "\n";
						mClientList.at(_clientIterator)->SetUsername(_message.substr(10));
						SendToBuffer(systemMessage);
						SendCustom(systemMessage);
					}
				}
			}
			else
			{
				if (IsUsernameValid(_message.substr(10)) == true)
				{
					mUsername = _message.substr(10);
				}
			}
		}
		////Change chat color
		//else if (_message.substr(7, 2) == "cc")
		//{
		//	//u:/cmd cc 255255255
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

				std::cout << "Whisper target: " << targetUsername << std::endl;

				//Check if username is a user on the server
				bool isValid{ false };
				int usernameIterator{ 0 };

				//Check if target is host
				if (targetUsername == mUsername)
				{
					isValid = true;
					usernameIterator = -1;
				}
				//Else iterate through the client list
				else
				{
					for (int x = 0; x < mClientList.size(); x++)
					{
						std::cout << mClientList.at(x)->GetUsername() << " -> " << targetUsername << std::endl;
						std::cout << (mClientList.at(x)->GetUsername() == targetUsername) << std::endl;

						//Check if target is user
						//if (mClientList.at(x)->GetUsername() == targetUsername);
						/// I have absolutely NO IDEA WHY, but using the line above ^ as the if statement doesn't work - usernames must be converted to c_str.
						/// An hour and a half lost debugging this silly crap
						if (strcmp(mClientList.at(x)->GetUsername().c_str(), targetUsername.c_str()) == 0)
						{
							std::cout << "Usernames Match!" << std::endl;
							isValid = true;
							usernameIterator = x;
							break;
						}
					}
				}

				//If username is valid
				if (isValid == true)
				{
					SendWhisper(_message.substr(6 + usernameEnd), _clientIterator, usernameIterator);
				}
				//Else, send warning
				else
				{
					SendWarning(INVALIDWHISPER, _clientIterator);
				}
			}
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
			if (IsUsernameValid(_message.substr(5), _clientIterator) == false)
			{
				SendCustom("s:/r q1", _clientIterator);
				mClientList.pop_back();
			}
			else
			{
				mClientList.at(_clientIterator)->SetUsername(_message.substr(5));
			}

			if (mClientList.size() == mServerSize)
			{
				SendCustom("s:/r q2", _clientIterator);
				mClientList.pop_back();
			}
		}
	}
	//If received message is a system response
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
	else
	{
		return false;
	}

	return true;
}

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

int User::GetServerResponse()
{
	return mServerResponse;
}

std::string User::GetWarningMessage()
{
	return mWarningMessage;
}

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

void User::SendToBuffer(std::string _message)
{
	mMessageBuffer = _message;
}