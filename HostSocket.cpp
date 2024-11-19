#include "HostSocket.h"

HostSocket::HostSocket()
{
}

HostSocket::HostSocket(int _port)
	: mSelectedSocket(INVALID_SOCKET)
{
	addrinfo *result = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Resolve address and port for server
	if (getaddrinfo(NULL, std::to_string(_port).c_str(), &hints, &result) != 0)
	{
		//WSACleanup();
		throw std::runtime_error("Failed to resolve server address or port");
	}

	//Create a socket for the server to listen with
	mSelectedSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (mSelectedSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		//WSACleanup();
		throw std::runtime_error("Failed to create socket");
	}

	//Setup the TCP listening socket
	if (bind(mSelectedSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		closesocket(mSelectedSocket);
		//WSACleanup();
		throw std::runtime_error("Failed to bind socket");
	}

	//addrinfo is no longer needed, free the memory
	freeaddrinfo(result);

	//Listen to the socket and wait for pending connections
	if (listen(mSelectedSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(mSelectedSocket);
		//WSACleanup();
		throw std::runtime_error("Failed to listen on socket");
	}
	//Set to non-blocking mode
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