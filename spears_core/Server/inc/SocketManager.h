#pragma once
#include <string>
#include <sys/socket.h>
#include "CommonTypes.h"

const size_t SIZE_CONTROL_SUM = 10;
const size_t SIZE_HASH_SUM = 20;

enum SocketError { EMPTY_ADDRESS, SOCKET_ERROR, CONNECTION_ERROR, BUFFER_ERROR, CONTROL_SUM_ERROR, HASH_SUM_ERROR, STRING_IS_EMPTY, IS_DONE };

class Protocol
{
public:
	std::pair<int, std::string> makeSendString(std::string message);
	std::pair<int, std::string> makeGetString(std::string message, size_t controlSum);
};

class SocketManager : public Protocol
{
private:
socklen_t sd;

protected:
	int sendMessage(std::string sendString);
	std::pair<int, std::string> getMessage();

public:
	SocketManager();
	int toConnect(std::string ip, std::string port);
	int hasConnection();
	int toDisconnect();
	void getSocket(socklen_t newSd);
};
