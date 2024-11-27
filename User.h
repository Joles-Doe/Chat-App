#pragma once
#include "Wsa.h"
#include "Client.h"
#include "HostSocket.h"
#include "ClientSocket.h"

#include <string>
#include <memory>
#include <vector>

enum Warnings
{
	INVALIDUSERNAME,
	INVALIDWHISPER,
	INVALIDCOLOR,
	INVALIDCOMMAND
};

enum Response
{
	WAITING,
	REJECT,
	ACCEPT,
	QUITINVALID,
	QUITFULL
};

class User
{
public:
	User() {};
	~User();

	void Update();

	void InitHost(std::string _username, int _serverSize);
	void InitClient(std::string _username, std::string _target);

	void Send(std::string _msg, int _userIterator = -1);
	void SendWhisper(std::string _msg, int _userIterator, int _targetUser);
	void SendWarning(Warnings _warning, int _targetUser);
	void SendCustom(std::string _msg, int _userIterator = -1);

	bool IsHost();

	std::string GetUsername();
	void SetUsername(std::string _username);

	bool Command(std::string _message, int _clientIterator = -1);

	void Reset();

	int GetServerResponse();
	std::string GetWarningMessage();

	std::string AddPrefix(int _clientIterator = -1);

	//Host related
	std::string GetSentMessage();
	int GetSentMessageColor(int _colorIndex);

	bool IsUsernameValid(std::string _username, int _clientIterator = -1);
	bool IsUsernameUnique(std::string _username, int _clientIterator = -1);

	std::string GetRoomCode();

private:
	Wsa wsa;

	HostSocket* host{ nullptr };
	ClientSocket* client{ nullptr };

	bool mIsHost{ false };

	std::string mUsername;

	//Warning related
	int mServerResponse{ 0 };
	std::string mWarningMessage;

	//Host related
	std::vector<std::shared_ptr<Client>> mClientList;
	bool mCommandCalled{ false };

	std::string mRoomCode;
	int mServerSize{ 0 };

	//Used by both
	void SendToBuffer(std::string _message);
	std::string mMessageBuffer;
	int mMessageColor[3]{ 0 };
};

