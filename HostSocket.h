#pragma once
#include "Wsa.h"
#include "ClientSocket.h"
#include <ws2tcpip.h>
#include <stdexcept>
#include <string>
#include <memory>

#include "Base36Tool.h"

//Server socket object

class HostSocket
{
public:
	HostSocket();
	HostSocket(int _port);
	~HostSocket();

	std::shared_ptr<ClientSocket> accept();

	std::string GetRoomCode();

private:
	SOCKET mSelectedSocket;
	HostSocket(const HostSocket& _copy);
	HostSocket& operator=(const HostSocket& _assign);

	std::string mRoomCode;
};

