#pragma once

#include <FL/Fl.H>

#include "ClientSocket.h"

#include <memory>
#include <string>

class Client
{
public:
	Client(std::shared_ptr<ClientSocket> _clientSocket);

	std::shared_ptr<ClientSocket> GetSocket();

	std::string GetUsername();
	void SetUsername(std::string _username);

	void SetChatColor(int _r, int _g, int _b);
	int GetChatColor(int _colorIndex);
private:
	std::shared_ptr<ClientSocket> mClientSocket;

	std::string mUsername;
	int mChatColor[3]{ 0 };
};