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

void Client::SetChatColor(int _r, int _g, int _b)
{
	mChatColor[0] = _r, mChatColor[1] = _g, mChatColor[2] = _b;
}

int Client::GetChatColor(int _colorIndex)
{
	int color{ 0 };
	if (_colorIndex > 0 && _colorIndex < 4)
	{
		color = mChatColor[_colorIndex];
	}

	return color;
}