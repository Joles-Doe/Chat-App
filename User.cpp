#include "User.h"

User::User()
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

void User::Update()
{
	if (host != nullptr)
	{
		std::shared_ptr<ClientSocket> client = host->accept();
		if (client)
		{
			std::cout << "Client Connected" << std::endl;
			mClientList.push_back(client);
		}

		for (size_t ci = 0; ci < mClientList.size(); ++ci)
		{
			std::string message;
			while (mClientList.at(ci)->Receive(message))
			{
				std::cout << "Client sent message: " << message.c_str() << std::endl;
				std::cout << "Sending to clients" << std::endl;

				for (size_t si = 0; si < mClientList.size(); ++si)
				{
					mClientList.at(si)->Send(message);
				}
			}
			SendToBuffer(message);
			if (mClientList.at(ci)->GetClosed())
			{
				std::cout << "Client Disconnected" << std::endl;
				mClientList.erase(mClientList.begin() + ci);
				--ci;
			}
		}
		std::cout << "Tick" << std::endl;
	}
}

void User::InitHost()
{
	host = new HostSocket(8080);
	std::cout << "Server listening on port 8080" << std::endl;
}

void User::InitClient()
{
	ClientSocket client("", 8080);
	std::cout << "Client connected to port 8080" << std::endl;
	
	std::string buffer;
	
}

void User::SendToBuffer(std::string _message)
{
	mMessageBuffer = _message;
}


std::string User::GetSentMessage()
{
	std::string send = mMessageBuffer;
	mMessageBuffer.clear();
	return send;
}

