#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdexcept>
#include <iostream>

class HostSocket;

class ClientSocket
{
public:
	ClientSocket();
	ClientSocket(std::string _IPaddress, int _port);
	~ClientSocket();
	bool Receive(std::string& _message);
	void Send(const std::string& _message);

	SOCKET GetSocket();
	void SetSocket(SOCKET _socket);

	bool GetClosed();
private:
	friend class HostSocket;

	SOCKET mSelectedSocket{ INVALID_SOCKET };
	bool mClosed{ false };

	ClientSocket(const ClientSocket& _copy);
	ClientSocket& operator=(const ClientSocket& _assign);
};