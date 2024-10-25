#include "HostSocket.h"

HostSocket::HostSocket(int _port)
	: mSelectedSocket(INVALID_SOCKET)
{
	addrinfo hints = { 0 };
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	addrinfo* result = NULL;
	if (getaddrinfo(NULL, std::to_string(_port).c_str(), &hints, &result) != 0)
	{
		throw std::runtime_error("Failed to resolve server address or port");
	}
	mSelectedSocket = socket(result->ai_family, result->ai_socktype,
		result->ai_protocol);
	if (mSelectedSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to create socket");
	}
	if (bind(mSelectedSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to bind socket");
	}
	freeaddrinfo(result);
	if (listen(mSelectedSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to listen on socket");
	}
	u_long mode = 1;
	if (ioctlsocket(mSelectedSocket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to set non-blocking");
	}
}
HostSocket::~HostSocket()
{
	closesocket(mSelectedSocket);
}

std::shared_ptr<ClientSocket> HostSocket::accept()
{
	SOCKET socket = ::accept(mSelectedSocket, NULL, NULL);
	if (socket == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			throw std::runtime_error("Failed to accept socket");
		}
		return std::shared_ptr<ClientSocket>();
	}
	std::shared_ptr<ClientSocket> rtn = std::make_shared<ClientSocket>();
	rtn->SetSocket(socket);
	return rtn;
}