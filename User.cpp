#include "User.h"

User::User()
{
}

User::~User()
{
}

void User::InitHost()
{
	HostSocket host(8080);
	std::cout << "Server listening on port 8080" << std::endl;

	std::vector<std::shared_ptr<ClientSocket>> clients;

	while (true)
	{
		std::shared_ptr<ClientSocket> client = host.accept();
		if (client)
		{
			std::cout << "Client Connected" << std::endl;
			clients.push_back(client);
		}

		for (size_t ci = 0; ci < clients.size(); ++ci)
		{
			std::string message;
			while (clients.at(ci)->Receive(message))
			{
				std::cout << "Client sent message: " << message.c_str() << std::endl;
				std::cout << "Sending to clients" << std::endl;
				for (size_t si = 0; si < clients.size(); ++si)
				{
					clients.at(si)->Send(message);
				}
			}
			if (clients.at(ci)->GetClosed())
			{
				std::cout << "Client Disconnected" << std::endl;
				clients.erase(clients.begin() + ci);
				--ci;
			}
		}
		std::cout << "Tick" << std::endl;
		Sleep(1000);
	}
}

void User::InitClient()
{
	ClientSocket client("", 8080);
	std::cout << "Client connected to port 8080" << std::endl;
	
	std::string buffer;
	
}
