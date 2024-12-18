#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
}

ClientSocket::ClientSocket(std::string _IPaddress, int _port, std::string _username)
{
	_IPaddress = Base36Tool::IntToIP(Base36Tool::Base36ToInt(_IPaddress));
	mRoomCode = _IPaddress;

	addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(_IPaddress.c_str(), std::to_string(_port).c_str(), &hints, &result) != 0)
	{
		throw std::runtime_error("Failed to find server. Did you type it in correctly?");
	}

	//Attempt to connect to the first address
	ptr = result;
	
	//Create a socket for connecting to the server
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		mSelectedSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (mSelectedSocket == INVALID_SOCKET)
		{
			freeaddrinfo(result);
			//WSACleanup();
			throw std::runtime_error("Failed to create socket. Try again later.");
		}

		//Enable non-blocking
		u_long mode = 1;
		if (ioctlsocket(mSelectedSocket, FIONBIO, &mode) == SOCKET_ERROR)
		{
			throw std::runtime_error("Failed to set non-blocking. Try again later.");
		}

		//Connect to server
		if (connect(mSelectedSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				closesocket(mSelectedSocket);
				mSelectedSocket = INVALID_SOCKET;
			}
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (mSelectedSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		//WSACleanup();
		throw std::runtime_error("Failed to connect to socket. Try again later.");
	}

	std::string userInformation = WrapInformation(_username);
	//Send user info buffer
	if (send(mSelectedSocket, userInformation.c_str(), userInformation.size(), 0) == SOCKET_ERROR)
	{
		closesocket(mSelectedSocket);
		//WSACleanup();
		throw std::runtime_error("Failed to connect. Try again later.");
	}
	//std::cout << "Sent bytes" << std::endl;
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
			mClosed = true;
			return false;
			//throw std::runtime_error("Read failed");
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

	//std::cout << bytes << std::endl;

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

void ClientSocket::Close()
{
	if (mSelectedSocket != INVALID_SOCKET)
	{
		closesocket(mSelectedSocket);
	}
}

std::string ClientSocket::GetRoomCode()
{
	return mRoomCode;
}

std::string ClientSocket::WrapInformation(std::string _username)
{
	if (_username == "")
	{
		_username = "GUEST";
	}

	std::string userInformation = "c:/s " + _username;
	return userInformation;
}
