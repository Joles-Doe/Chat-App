#pragma once
#include "Wsa.h"
#include "HostSocket.h"
#include "ClientSocket.h"

#include <string>
#include <memory>
#include <vector>

class User
{
public:
	User() {};
	~User();

	void Update();

	void InitHost();
	void InitClient();

	void Send(std::string _msg, int _userIterator = -1);

	//Host related
	std::string GetSentMessage();
private:
	Wsa wsa;

	HostSocket* host{ nullptr };
	ClientSocket* client{ nullptr };


	//Host related
	std::vector<std::shared_ptr<ClientSocket>> mClientList;

	void SendToBuffer(std::string _message);
	std::string mMessageBuffer;
};

