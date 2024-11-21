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
	if (host != nullptr)
	{
		mCommandCalled = false;

		//Check if someone new has joined
		std::shared_ptr<ClientSocket> client = host->accept();
		if (client)
		{
			std::cout << "Client Connected" << std::endl;
			mClientList.push_back(std::make_shared<Client>(client));

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
				if (Command(message, ci) == false)
				{
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
		//std::cout << "Tick" << std::endl;
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

void User::InitHost(std::string _username)
{
	if (_username == "")
	{
		mUsername = "HOST";
	}
	else
	{
		mUsername = _username;
	}

	if (host != nullptr)
	{
		delete host;
	}
	host = new HostSocket(8080);
}

void User::InitClient(std::string _username)
{
	if (_username == "")
	{
		mUsername = "GUEST";
	}
	else
	{
		mUsername = _username;
	}

	if (client != nullptr)
	{
		delete client;
	}
	client = new ClientSocket("", 8080, mUsername);
}

void User::Send(std::string _msg, int _userIterator)
{
	if (host != nullptr)
	{
		std::cout << "Send " <<  _userIterator << std::endl;

		_msg = AddPrefix(_userIterator) + _msg + "\n";

		for (size_t si = 0; si < mClientList.size(); ++si)
		{
			if (si != _userIterator)
			{
				mClientList.at(si)->GetSocket()->Send(_msg);
			}
		}
	}
	else if (client != nullptr)
	{
		client->Send(_msg);
	}
}

void User::SendWhisper(std::string _msg, int _userIterator, int _targetUser)
{
	if (host != nullptr)
	{
		std::cout << "Whisper to " << _targetUser << "from " << _userIterator << std::endl;

		_msg = "[" + AddPrefix(_userIterator) + _msg + "]\n";
		mClientList.at(_targetUser)->GetSocket()->Send(_msg);
	}
}

void User::SendWarning(int _warning, int _targetUser)
{
	if (host != nullptr)
	{
		std::cout << "Warning to " << _targetUser << "with Warning: " << _warning << std::endl;

		//username
		if (_warning == 0)
		{
			mClientList.at(_targetUser)->GetSocket()->Send("s:/w 0");
		}
		//whisper
		else if (_warning == 1)
		{

		}
	}
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

bool User::IsUsernameUnique(std::string _username, int _clientIterator)
{
	if(_clientIterator == -1)
	{
		
	}
	else
	{
		
	}

	return false;
}

bool User::Command(std::string _message, int _clientIterator)
{
	//If received message is a command
	if (_message.substr(0, 6) == "u:/cmd")
	{
		std::cout << "COMMAND RECEIVED" << std::endl;

		if (_message.substr(7, 2) == "cu")
		{
			std::cout << "CHANGE USERNAME" << std::endl;
			if (_clientIterator != -1 && host != nullptr)
			{
				std::cout << mClientList.at(_clientIterator)->GetUsername() << std::endl;
				if (IsUsernameUnique(_message.substr(10), _clientIterator) == true)
				{
					mClientList.at(_clientIterator)->SetUsername(_message.substr(10));
				}
				
			}
			else
			{
				SetUsername(_message.substr(10));
			}
		}
	}
	//If received message is a whisper
	else if (_message.substr(0, 4) == "u:/w")
	{

	}
	//If received message is a warning
	else if (_message.substr(0, 4) == "s:/w")
	{

	}
	else
	{
		return false;
	}



	return true;
}

std::string User::AddPrefix(int _clientIterator)
{
	std::string prefix;
	if (_clientIterator == -1 || host == nullptr)
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