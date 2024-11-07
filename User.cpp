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
		//Check if someone new has joined
		std::shared_ptr<ClientSocket> client = host->accept();
		if (client)
		{
			std::cout << "Client Connected" << std::endl;
			mClientList.push_back(client);
		}

		//For each client, check if there's new data
		for (size_t ci = 0; ci < mClientList.size(); ++ci)
		{
			//Check if client has sent a message
			std::string message;
			while (mClientList.at(ci)->Receive(message))
			{
				std::cout << "Client sent message: " << message.c_str() << std::endl;
				//Send that message to the other clients
				Send(message, ci);
			}

			//Send the message to the host's buffer
			SendToBuffer(message);

			/// NOTE TO SELF
			/// CHARACTER LIMIT IS 125 CHARACTERS, ENSURE CHAT IS LIMITED TO 125
			
			//Check if client has disconnected
			if (mClientList.at(ci)->GetClosed())
			{
				std::cout << "Client Disconnected" << std::endl;
				mClientList.erase(mClientList.begin() + ci);
				--ci;
			}
		}
		//std::cout << "Tick" << std::endl;
	}
}

void User::InitHost()
{
	host = new HostSocket(8080);
}

void User::InitClient()
{
	ClientSocket client("", 8080);	
}

void User::Send(std::string _msg, int _userIterator)
{
	for (size_t si = 0; si < mClientList.size(); ++si)
	{
		if (si != _userIterator)
		{
			mClientList.at(si)->Send(_msg);
		}
	}
}

std::string User::GetSentMessage()
{
	std::string send = mMessageBuffer;
	mMessageBuffer.clear();
	return send;
}

void User::SendToBuffer(std::string _message)
{
	mMessageBuffer = _message;
}