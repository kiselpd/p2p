#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <boost/scoped_ptr.hpp>
#include <arpa/inet.h>

#include "SocketManager.h"

std::pair<int, std::string> Protocol::makeSendString(std::string message)
{
	std::pair<int, std::string> result;

	if (message.empty())
	{
		result.first = STRING_IS_EMPTY;
		result.second = message;

		return result;
	}

	std::hash<std::string> hashSum;
	std::string hashSumString = std::to_string(hashSum(message));
	std::string sendString(SIZE_CONTROL_SUM + SIZE_HASH_SUM, '0');

	sendString.replace(SIZE_CONTROL_SUM + SIZE_HASH_SUM - hashSumString.size(), hashSumString.size(), hashSumString);

	std::string controlSum = std::to_string((hashSumString + message).size());
	sendString.replace(SIZE_CONTROL_SUM - controlSum.size(), controlSum.size(), controlSum);

	result.first = IS_DONE;
	result.second = sendString + message;

	return result;
}

std::pair<int, std::string> Protocol::makeGetString(std::string message, size_t controlSum)
{
	std::pair<int, std::string> result;

	if (message.size() != controlSum)
	{
		result.first = CONTROL_SUM_ERROR;
		result.second = message;

		return result;
	}

	if (message.empty())
	{
		result.first = STRING_IS_EMPTY;
		result.second = message;

		return result;
	}

	std::hash<std::string> hashSum;

	std::string controlHashSumString = message.substr(0, SIZE_HASH_SUM);
	std::string getMessage = message.substr(SIZE_HASH_SUM);

	std::string hashSumString(SIZE_HASH_SUM, '0');
	std::string hash = std::to_string(hashSum(getMessage));
	hashSumString.replace(SIZE_HASH_SUM - hash.size(), hash.size(), hash);

	if (controlHashSumString == hashSumString)
	{
		result.first = IS_DONE;
		result.second = getMessage;
	}
	else
	{
		result.first = HASH_SUM_ERROR;
		result.second = getMessage;
	}

	return result;
}

SocketManager::SocketManager()
{
	this->sd = socket(PF_INET, SOCK_STREAM, 0);
}

void SocketManager::getSocket(socklen_t newSd)
{
	this->sd = newSd;
}

int SocketManager::toConnect(std::string ip, std::string port)
{
	if (ip.empty())
		return EMPTY_ADDRESS;

	if (port.empty())
		return EMPTY_ADDRESS;

	if (sd < 0)
		return SOCKET_ERROR;

	const char* ip_char = ip.c_str();

	sockaddr_in peer;
	hostent* hp = gethostbyname(ip_char);

	peer.sin_family = AF_INET;
	peer.sin_port = htons(std::stoi(port));
	memcpy(&peer.sin_addr, hp->h_addr, hp->h_length);

	int connection = connect(this->sd, (sockaddr*)&peer, sizeof(peer));

	if (connection)
		return CONNECTION_ERROR;

	return IS_DONE;
}

int SocketManager::hasConnection()
{
	if (this->sd < 0)
		return SOCKET_ERROR;
	else
		return IS_DONE;
}

int SocketManager::toDisconnect()
{
	int result = close(this->sd);          
	if (result)
		return SOCKET_ERROR;
	else
		return IS_DONE;
}

int SocketManager::sendMessage(std::string sendString)
{
	if (sendString.empty())
		return STRING_IS_EMPTY;

	if (sd < 0)
		return SOCKET_ERROR;

	char* buffer = new char[sendString.size()];

	if (!buffer)
		return BUFFER_ERROR;

	if (!strcpy(buffer, sendString.c_str()))
	{
		delete[] buffer;
		return BUFFER_ERROR;
	}

	int statusMessage = send(this->sd, buffer, sendString.size(), 0);

	if (statusMessage < sendString.size())
	{
		delete[] buffer;
		return SOCKET_ERROR;
	}

	delete[] buffer;
	return IS_DONE;
}

std::pair<int, std::string> SocketManager::getMessage()
{
	std::pair<int, std::string> result;

	if (this->sd < 0)
	{
		result.first = -1;
		return result;
	}

	int recvSize = 0;
	char* bufSizeString = new char[SIZE_CONTROL_SUM];

	recvSize = recv(this->sd, bufSizeString, SIZE_CONTROL_SUM, 1);

	if (recvSize > 0)
	{
		int sizeString = std::stoi(bufSizeString);
		char* bufString = new char[sizeString];
		recvSize = recv(sd, bufString, sizeString, 1);

		if (recvSize > 0)
		{
			result.first = std::stoi(bufSizeString);
			result.second = bufString;
		}
		else
			result.first = -1;

		delete[] bufString;
	}
	else
		result.first = -1;

	delete[] bufSizeString;

	return result;
}
