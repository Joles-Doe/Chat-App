#pragma once
#include "Wsa.h"
#include "ClientSocket.h"
#include "Base36Tool.h"
#include <ws2tcpip.h>
#include <stdexcept>
#include <string>
#include <memory>

/*
  Socket class responsible for server-side data handling.

  Upon construction, opens a socket in the port specified.
  Operates in non-blocking format.
*/
class HostSocket
{
public:
	HostSocket(int _port);
	~HostSocket();

	//Attempts to accept an incoming connection, returns a client socket if true, or an empty client socket
	std::shared_ptr<ClientSocket> Accept();

	//Closes socket - queues incoming data
	void Close();

	//Returns created Room Code
	std::string GetRoomCode();

private:
	SOCKET mSelectedSocket;
	HostSocket(const HostSocket& _copy);
	HostSocket& operator=(const HostSocket& _assign);

	std::string mRoomCode;
};

