#pragma once
#include <WinSock2.h>

class Wsa
{
public:
	Wsa();
	~Wsa();

private:
	WSADATA wsaData{ 0 };

	Wsa(const Wsa& _copy);
	Wsa& operator=(const Wsa& _assign);
};