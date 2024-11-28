#pragma once

#include <FL/Fl.H>

#include "ClientSocket.h"

#include <memory>
#include <string>

/*
  Client class made to hold information about a client, includes their socket and their username.

  Should not be called directly, and should only be used by the host when accepting a new client into their chatroom.
*/
class Client
{
public:
	Client(std::shared_ptr<ClientSocket> _clientSocket);

	//Returns a pointer to the socket class
	std::shared_ptr<ClientSocket> GetSocket();

	//Returns username
	std::string GetUsername();
	//Sets username
	void SetUsername(std::string _username);

private:
	std::shared_ptr<ClientSocket> mClientSocket;

	std::string mUsername;
};