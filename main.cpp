#include <iostream>
#include <vector>
#include <FL/Fl.H>
#include "MainWindow.h"
#include "User.h"
#include "Wsa.h"
#include "HostSocket.h"
#include "ClientSocket.h"
#include <conio.h>

void Server();
void Connect();

int main(int argc, char* argv[]) //Ensure main has these parameters
{
	//Set theming
	Fl::scheme("gtk+"); //Must be called before any other FLTK function

	//Initialize winsock
	Wsa wsa;
	std::cout << "Initialized Winsock" << std::endl;

	//Initialize window
	MainWindow win;
	std::cout << "Initialized Window" << std::endl;

	//Pass a user reference to the FLTK window
	std::shared_ptr<User> user = std::make_shared<User>();
	win.SetUser(user);

	//Display the window
	win.show(argc, argv); //For the first win.show function, pass argc and argv as parameters

	return Fl::run(); //Must be called at the end of main()
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
	ClientSocket client("", 8080);
	std::cout << "Client connected to port 8080" << std::endl;

	std::string buffer;

	while (true)
	{
		char ch;
		do
		{
			ch = _getch();
			buffer = ch;
			client.Send(buffer);
		} while (ch != 'q');
	}
}
