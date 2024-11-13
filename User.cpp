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
		}

		//For each client, check if there's new data
		for (size_t ci = 0; ci < mClientList.size(); ++ci)
		{
			//Check if client has sent a message
			std::string message;
			while (mClientList.at(ci)->GetSocket()->Receive(message))
			{
				std::cout << "Client sent message: " << message.c_str() << std::endl;

				mCommandCalled = Command(message, ci);
				if (mCommandCalled == false)
				{
					//Send that message to the other clients
					Send(message, ci);
				}				
			}

			if (mCommandCalled == false)
			{
				//Send the message to the host's buffer
				SendToBuffer(message);
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
	client = new ClientSocket("", 8080, mUsername);
}

void User::Send(std::string &_msg, int _userIterator)
{
	if (host != nullptr)
	{
		std::cout << "Send " <<  _userIterator << std::endl;

		//Check if host was sending the message
		if (_userIterator == -1)
		{
			_msg = mUsername + ": " + _msg;
		}
		else
		{
			_msg = mClientList.at(_userIterator)->GetUsername() + ": " + _msg;
		}

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
		std::string rawMessage = _msg;
		_msg = mUsername + ": " + _msg;
		client->Send(rawMessage);
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

bool User::Command(std::string _message, int _clientIterator)
{
	//If received message is a command
	if (_message.substr(0, 4) == "/cmd")
	{
		std::cout << "COMMAND RECEIVED" << std::endl;
		/*
		Commands
		User Setup
		Change username
		*/
		//Command Change Username

		if (_message.substr(5, 2) == "cu")
		{
			std::cout << "CHANGE USERNAME" << std::endl;
			if (_clientIterator != -1)
			{
				mClientList.at(_clientIterator)->SetUsername(_message.substr(8));
			}
			else
			{
				SetUsername(_message.substr(8));
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

void User::SendToBuffer(std::string _message)
{
	mMessageBuffer = _message;
}