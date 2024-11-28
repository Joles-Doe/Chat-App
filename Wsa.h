#pragma once
#include <WinSock2.h>

/*
  Struct used to automatically construct and destruct the WSA library
*/
struct Wsa
{
	Wsa();
	Wsa(const Wsa& _copy);
	~Wsa();
	Wsa& operator=(const Wsa& _assign);

	WSADATA wsaData{ 0 };
};