#include "Client.h"


Client::Client(std::shared_ptr<ClientSocket> _clientSocket)
{
	mClientSocket = _clientSocket;
}

std::shared_ptr<ClientSocket> Client::GetSocket()
{
	return mClientSocket;
}

std::string Client::GetUsername()
{
	return mUsername;
}

void Client::SetUsername(std::string _username)
{
	mUsername = _username;
}