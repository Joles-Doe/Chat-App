#include "HostSocket.h"

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
	if (getaddrinfo("localhost", std::to_string(_port).c_str(), &hints, &result) != 0)
	{
		throw std::runtime_error("Failed to resolve server address or port");
	}

	//Create a socket for the server to listen with
	mSelectedSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (mSelectedSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		throw std::runtime_error("Failed to create socket");
	}

	//Setup the TCP listening socket
	if (bind(mSelectedSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		closesocket(mSelectedSocket);
		throw std::runtime_error("Failed to bind socket");
	}

	//Get the local IP address after binding
	sockaddr_in localAddress;
	socklen_t localAddressLen = sizeof(localAddress);
	if (getsockname(mSelectedSocket, (sockaddr*)&localAddress, &localAddressLen) == SOCKET_ERROR)
	{
		freeaddrinfo(result);
		closesocket(mSelectedSocket);
		throw std::runtime_error("Failed to get local address");
	}

	//Convert the IP address to a string
	char ipStr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &localAddress.sin_addr, ipStr, sizeof(ipStr)) == nullptr)
	{
		freeaddrinfo(result);
		closesocket(mSelectedSocket);
		throw std::runtime_error("Failed to convert IP address");
	}

	std::string IP = ipStr;
	//std::cout << "Local IP Address: " << IP << std::endl;
	mRoomCode = Base36Tool::IntToBase36(Base36Tool::IPToInt(IP));
	//std::cout << "Room Code: " << mRoomCode << std::endl;
	//std::cout << "Decrypted: " << Base36Tool::IntToIP(Base36Tool::Base36ToInt(mRoomCode)) << std::endl;

	//addrinfo is no longer needed, free the memory
	freeaddrinfo(result);

	//Listen to the socket and wait for pending connections
	if (listen(mSelectedSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(mSelectedSocket);
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

std::shared_ptr<ClientSocket> HostSocket::Accept()
{
	//Attempt to accept an incoming connection
	SOCKET socket = ::accept(mSelectedSocket, NULL, NULL);

	//If no connections are present
	if (socket == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			throw std::runtime_error("Failed to accept socket");
		}
		return std::shared_ptr<ClientSocket>();
	}

	//Create a client socket and return it
	std::shared_ptr<ClientSocket> rtn = std::make_shared<ClientSocket>();
	rtn->SetSocket(socket);
	return rtn;
}

void HostSocket::Close()
{
	if (mSelectedSocket != INVALID_SOCKET)
	{
		closesocket(mSelectedSocket);
	}
}

std::string HostSocket::GetRoomCode()
{
	return mRoomCode;
}