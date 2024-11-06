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

	void Update();

	void InitHost();
	void InitClient();

	std::string GetSentMessage();
private:
	Wsa wsa;

	HostSocket* host{ nullptr };
	ClientSocket* client{ nullptr };

	std::vector<std::shared_ptr<ClientSocket>> mClientList;

	void SendToBuffer(std::string _message);
	std::string mMessageBuffer;
};

