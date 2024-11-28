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
	QUITFULL,
	DISCONNECT
};

/*
  Class used to represent the user.
  
  Acts as either server host or client, dependent on which is initialised.
  Contains all the necessary logic for the networking side of the program.
*/
class User
{
public:
	User(std::string _version);
	~User();

	//Update function - automatically called once every tick.
	void Update();

	//Initialises the Host part of the class - creates a server and allows incoming connections from other users
	void InitHost(std::string _username, int _serverSize);
	//Initialises the Client part of the class - attempts to join a server using the address given
	void InitClient(std::string _username, std::string _target);

	//General send function - sends a message to every user except the one specified
	void Send(std::string _msg, int _userIterator = -1);
	//Whisper send function - sends a message to the targeted user
	void SendWhisper(std::string _msg, int _userIterator, int _targetUser);
	//Warning send function - used by the server to respond to an invalid request
	void SendWarning(Warnings _warning, int _targetUser);
	//Custom send function - used by the server to respond in a particular way
	void SendCustom(std::string _msg, int _userIterator = -1);

	//Command function - used primarily by the server to respond to requests from the user. Typically involves commands such as changing username, etc.
	bool Command(std::string _message, int _clientIterator = -1);

	//Returns if the user is the host
	bool IsHost();

	//Returns stored username
	std::string GetUsername();
	//Sets username
	void SetUsername(std::string _username);


	//Resets variables
	void Reset();

	//Returns server response
	int GetServerResponse();
	//Returns server message
	std::string GetWarningMessage();

	//Prefix function, used by the server to prefix the sender's username before the message is sent to other users
	std::string AddPrefix(int _clientIterator = -1);
	//Whisper prefix function, used by the server to custom prefix a whisper before it is sent
	std::string AddWhisperPrefix(int _clientIterator = -1, int _targetUser = -1);

	//Returns received message
	std::string GetSentMessage();

	//Username checker - checks if a username fits the requirements set by the server
	bool IsUsernameValid(std::string _username, int _clientIterator = -1);
	//Username checker - checks if a username is unique
	bool IsUsernameUnique(std::string _username, int _clientIterator = -1);
	
	//Returns room code
	std::string GetRoomCode();

	//Returns chat version
	std::string GetChatVersion();

private:
	Wsa wsa;

	HostSocket* host{ nullptr };
	ClientSocket* client{ nullptr };

	bool mIsHost{ false };
	bool mServerOpen{ false };

	std::string mUsername;

	bool mRemoveUser{ false };
	int mServerResponse{ 0 };
	std::string mWarningMessage;

	std::vector<std::shared_ptr<Client>> mClientList;
	bool mCommandCalled{ false };

	std::string mRoomCode;
	int mServerSize{ 0 };

	//Function used to send a message to the user's local buffer
	void SendToBuffer(std::string _message);
	std::string mMessageBuffer;

	std::string mChatVersion;
};

