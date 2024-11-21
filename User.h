#pragma once
#include "Wsa.h"
#include "Client.h"
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

	void InitHost(std::string _username = "");
	void InitClient(std::string _username = "");

	void Send(std::string _msg, int _userIterator = -1);

	std::string GetUsername();
	void SetUsername(std::string _username);

	bool Command(std::string _message, int _clientIterator = -1);

	std::string AddPrefix(std::string _message, int _clientIterator = -1);

	//Host related
	std::string GetSentMessage();
private:
	Wsa wsa;

	HostSocket* host{ nullptr };
	ClientSocket* client{ nullptr };

	std::string mUsername;

	//Host related
	std::vector<std::shared_ptr<Client>> mClientList;
	bool mCommandCalled{ false };

	//Used by both
	void SendToBuffer(std::string _message);
	std::string mMessageBuffer;
};

