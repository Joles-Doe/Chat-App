#pragma once
#include "HostSocket.h"
#include "ClientSocket.h"
#include <memory>
#include <vector>
#include "Wsa.h"

class User
{
public:
	User();
	~User();

	void InitHost();
	void InitClient();
private:
	Wsa wsa;

	HostSocket host;
	ClientSocket client;
};

