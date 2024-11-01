#pragma once
#include <WinSock2.h>

// Winsocket data container
struct Wsa
{
	Wsa();
	Wsa(const Wsa& _copy);
	~Wsa();
	Wsa& operator=(const Wsa& _assign);

	WSADATA wsaData{ 0 };
};