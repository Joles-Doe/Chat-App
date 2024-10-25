#include "Wsa.h"
#include <stdexcept>

Wsa::Wsa()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		throw std::runtime_error("Failed to initialize Winsock");
	}
}

Wsa::~Wsa()
{
	WSACleanup();
}
