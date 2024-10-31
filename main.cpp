#include <iostream>
#include <vector>
#include "Wsa.h"
#include "HostSocket.h"
#include "ClientSocket.h"

void Server();
void Connect();

int main(int argc, char* argv[]) //Ensure main has these parameters
{
	////Set theming
	//Fl::scheme("gtk+"); //Must be called before any other FLTK function

	//MainWindow win;

	////Display the window
	//win.show(argc, argv); //For the first win.show function, pass argc and argv as parameters

	//return Fl::run(); //Must be called at the end of main()

	Wsa wsa;
	std::cout << "Initialized Winsock" << std::endl;

	int choice{ 0 };
	do
	{
		std::cout << "1. Server\n2. Client" << std::endl;
		std::cin >> choice;

		switch (choice)
		{
		case 1:
			Server();
			break;
		case 2:
			Connect();
		default:
			break;
		}
	} while (choice != (1 || 2));

	
}

void Server()
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

void Connect()
{
	ClientSocket client(8080);
	std::cout << "Client connected to port 8080" << std::endl;

	std::string buffer;

	while (true)
	{
		std::cin >> buffer;
		client.Send(buffer);
	}
}
