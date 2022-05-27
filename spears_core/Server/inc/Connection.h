#pragma once
#include <string>
#include "SocketManager.h"
#include "Parser.h"
#include "CommonTypes.h"
#include "Crypto.h"

class GlobalServer : public SocketManager
{
private:
	Crypto* cr;

public:
	std::string myLogin;
	std::string publicKey;
	std::string ip;
	std::string port;

public:
	GlobalServer(Crypto *cr);
	GlobalServer();
	std::pair<int, serverMessage> getMessage();
	int sendAuthorizationRequest(std::string localClientLogin, std::string localClientPassword);
	int sendRegistrationRequest(std::string localClientLogin, std::string localClientPassword);
	int sendSearchRequest(std::string clientLogin);
	int sendPublicKey(std::string publicKey);
};

class Client : public SocketManager
{
private:
	Crypto* cr;
	std::string crypto;

public:
	std::string clientLogin;
	std::string publicKey;

public:
	Client(Crypto *cr);
	Client();
	std::pair<int, clientMessage> getMessage();
	int sendMessage(std::string textMessage, std::string timeMessage);
	int sendFriendshipRequest(RequestStatus status);
};
