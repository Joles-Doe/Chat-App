#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdexcept>
#include <iostream>

#include "Base36Tool.h"

class HostSocket;

class ClientSocket
{
public:
	ClientSocket();
	ClientSocket(std::string _IPaddress, int _port, std::string _username = "");
	~ClientSocket();
	bool Receive(std::string& _message);
	void Send(const std::string& _message);

	SOCKET GetSocket();
	void SetSocket(SOCKET _socket);

	bool GetClosed();
	void Close();

	std::string GetRoomCode();
private:
	friend class HostSocket;

	SOCKET mSelectedSocket{ INVALID_SOCKET };
	bool mClosed{ false };
	
	std::string WrapInformation(std::string _username);

	std::string mRoomCode;

	ClientSocket(const ClientSocket& _copy);
	ClientSocket& operator=(const ClientSocket& _assign);
};