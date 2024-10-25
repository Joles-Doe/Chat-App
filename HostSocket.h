#pragma once
#include "Wsa.h"
#include "ClientSocket.h"
#include <ws2tcpip.h>
#include <stdexcept>
#include <string>
#include <memory>

class HostSocket
{
public:
	HostSocket(int _port);
	~HostSocket();

	std::shared_ptr<ClientSocket> accept();

private:
	SOCKET mSelectedSocket;
	HostSocket(const HostSocket& _copy);
	HostSocket& operator=(const HostSocket& _assign);
};

