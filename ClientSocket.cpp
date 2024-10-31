#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
}

ClientSocket::ClientSocket(int _port)
{
	addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Replace NULL with IP address when not connecting locally
	if (getaddrinfo(NULL, std::to_string(_port).c_str(), &hints, &result) != 0)
	{
		throw std::runtime_error("Failed to resolve server address or port");
	}

	//Attempt to connect to the first address
	ptr = result;
	//Create a socket for connecting to the server
	mSelectedSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);
	if (mSelectedSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		throw std::runtime_error("Failed to create socket");
	}
	if (connect(mSelectedSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to connect to socket");
	}
	freeaddrinfo(result);
	
	if (send(mSelectedSocket, "TEST", 4, 0) == SOCKET_ERROR)
	{
		closesocket(mSelectedSocket);
		WSACleanup();
		throw std::runtime_error("Failed to send");
	}
}

ClientSocket::~ClientSocket()
{
	if (mSelectedSocket != INVALID_SOCKET)
	{
		closesocket(mSelectedSocket);
	}
}

bool ClientSocket::Receive(std::string& _message)
{
	char buffer[128] = { 0 };
	int bytes = recv(mSelectedSocket, buffer, sizeof(buffer) - 1, 0);
	if (bytes == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			throw std::runtime_error("Read failed");
		}
		return false;
	}
	else if (bytes == 0)
	{
		mClosed = true;
		return false;
	}
	_message = buffer;

	return true;
}

void ClientSocket::Send(const std::string& _message)
{
	int bytes = send(mSelectedSocket, _message.c_str(), _message.length(), 0);
	if (bytes <= 0)
	{
		throw std::runtime_error("Failed to send data");
	}
}

SOCKET ClientSocket::GetSocket()
{
	return mSelectedSocket;
}

void ClientSocket::SetSocket(SOCKET _socket)
{
	mSelectedSocket = _socket;
}

bool ClientSocket::GetClosed()
{
	return mClosed;
}
