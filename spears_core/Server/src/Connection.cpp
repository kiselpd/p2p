#include <cstring>
#include <ctime>
#include "Connection.h"
#include "SocketManager.h"
#include "Parser.h"
#include "Crypto.h"
#include "CommonTypes.h"

GlobalServer::GlobalServer(Crypto *cr) : cr(cr) {};
Client::Client(Crypto *cr) : cr(cr) {};

GlobalServer::GlobalServer(){};
Client::Client() {};

int GlobalServer::sendAuthorizationRequest(std::string localClientLogin, std::string localClientPassword) 
{
	if (!this->SocketManager::hasConnection())
		return CONNECTION_ERROR;

	serverMessage sendMessage;
	
	time_t now = time(0); 
	tm* gmtm = gmtime(&now);
	char* dt = asctime(gmtm);
	
	sendMessage.time = dt;
	sendMessage.login = localClientLogin;
	sendMessage.type = AUTHORIZATION;
	sendMessage.text = localClientPassword;

	std::string sendString;

	sendString = sendMessage.parse();
	//sendString = cr->encrypt(sendString);

	std::pair<int, std::string> sendPair = this->Protocol::makeSendString(sendString);

	if (sendPair.first == STRING_IS_EMPTY)
		return REQUEST_ERROR;

	int result = this->SocketManager::sendMessage(sendPair.second);

	if (result != IS_DONE)
		return REQUEST_ERROR;
		
	return SUCCESSFUL_REQUEST;
}

int GlobalServer::sendRegistrationRequest(std::string localClientLogin, std::string localClientPassword) 
{
	if (!this->hasConnection())
		return CONNECTION_ERROR;

	serverMessage sendMessage;

	time_t now = time(0);
	tm* gmtm = gmtime(&now);
	char* dt = asctime(gmtm);

	sendMessage.time = dt;
	sendMessage.text = localClientPassword;
	sendMessage.login = localClientLogin;
	sendMessage.type = REGISTRATION;

	std::string sendString;

	sendString = sendMessage.parse();
	//sendString = cr->encrypt(sendString);

	std::pair<int, std::string> sendPair = this->Protocol::makeSendString(sendString);

	if (sendPair.first == STRING_IS_EMPTY)
		return REQUEST_ERROR;

	int result = this->SocketManager::sendMessage(sendPair.second);

	if (result != IS_DONE)
		return REQUEST_ERROR;

	return SUCCESSFUL_REQUEST;
}

int GlobalServer::sendSearchRequest(std::string clientLogin) 
{
	std::pair<int, std::string> resultPair;

	if (!this->hasConnection())
		return CONNECTION_ERROR;
	/*
	if (this->myLogin.empty())
		return NOT_AUTORIZED;
*/
	serverMessage sendMessage;

	time_t now = time(0);
	tm* gmtm = gmtime(&now);
	char* dt = asctime(gmtm);
	sendMessage.time = dt;

	sendMessage.text = clientLogin;
	sendMessage.login = this->myLogin;
	sendMessage.type = SEARCH_FRIEND;

	std::string sendString;

	sendString = sendMessage.parse();
	//sendString = cr->encrypt(sendString);

	std::pair<int, std::string> sendPair = this->Protocol::makeSendString(sendString);

	if (sendPair.first == STRING_IS_EMPTY)
		return REQUEST_ERROR;

	int result = this->SocketManager::sendMessage(sendPair.second);

	if (result != IS_DONE)
		return REQUEST_ERROR;

	return SUCCESSFUL_REQUEST;
}

int GlobalServer::sendPublicKey(std::string publicKey)
{
	std::pair<int, std::string> resultPair;

	if (!this->hasConnection())
		return CONNECTION_ERROR;

	serverMessage sendMessage;

	time_t now = time(0);
	tm* gmtm = gmtime(&now);
	char* dt = asctime(gmtm);

	sendMessage.time = dt;
	sendMessage.text = publicKey;
	sendMessage.type = SEND_PUBLIC_KEY;

	std::string sendString;

	sendString = sendMessage.parse();

	std::pair<int, std::string> sendPair = this->Protocol::makeSendString(sendString);

	if (sendPair.first == STRING_IS_EMPTY)
		return REQUEST_ERROR;

	int result = this->SocketManager::sendMessage(sendPair.second);

	if (result != IS_DONE)
		return REQUEST_ERROR;

	return SUCCESSFUL_REQUEST;
}

int Client::sendMessage(std::string textMessage, std::string timeMessage)
{
	std::pair<int, std::string> resultPair;

	if (!this->hasConnection())
		return CONNECTION_ERROR;

	clientMessage sendMessage;

	sendMessage.time = timeMessage; 
	sendMessage.text = textMessage;
	sendMessage.cryptoWord = this->crypto; 

	std::string sendString;

	sendString = sendMessage.parse();
	//sendString = cr->encrypt(sendString);

	std::pair<int, std::string> sendPair = this->Protocol::makeSendString(sendString);

	if (sendPair.first == STRING_IS_EMPTY)
		return REQUEST_ERROR;

	int result = this->SocketManager::sendMessage(sendPair.second);

	if (result != IS_DONE)
		return REQUEST_ERROR;

	return SUCCESSFUL_REQUEST;
}

int Client::sendFriendshipRequest(RequestStatus status)
{
	std::pair<int, std::string> resultPair;

	if (!this->hasConnection())
		return CONNECTION_ERROR;

	clientMessage sendMessage;

	time_t now = time(0);
	tm* gmtm = gmtime(&now);
	char* dt = asctime(gmtm);

	sendMessage.time = dt; 

	switch (status)
	{
	case SUCCESSFUL_REQUEST:
	{
		sendMessage.text = (cr->get_key(PUBLIC_KEY)) + "/!@#$%^&*CoNnEcT*&^%$#@!/" + (this->clientLogin);
		break;
	}
	case REQUEST_ERROR:
	{
		sendMessage.text = (cr->get_key(PUBLIC_KEY)) + "/!@#$%^&*No_CoNnEcT*&^%$#@!/" + (this->clientLogin);
		break;
	}
	default:
		return REQUEST_ERROR;
	}

	sendMessage.cryptoWord = this->crypto; 

	std::string sendString;

	sendString = sendMessage.parse();
	//sendString = cr->encrypt(sendString);

	std::pair<int, std::string> sendPair = this->Protocol::makeSendString(sendString);

	if (sendPair.first == STRING_IS_EMPTY)
		return REQUEST_ERROR;

	int result = this->SocketManager::sendMessage(sendPair.second);

	if (result != IS_DONE)
		return REQUEST_ERROR;

	return SUCCESSFUL_REQUEST;
}

std::pair<int, clientMessage> Client::getMessage()
{
	std::pair<int, clientMessage> resultPair;

	if (!this->hasConnection())
	{
		resultPair.first = CONNECTION_ERROR;
		return resultPair;
	}
	
	std::pair<int, std::string> resultString = this->SocketManager::getMessage();

	if (resultString.first > 0)
	{
		std::pair<int, std::string> getPair = this->Protocol::makeGetString(resultString.second, resultString.first);

		std::string getString = cr->decrypt(getPair.second);

		clientMessage getMessage;

		get_request_body(getString, getMessage);

		resultPair.first = SUCCESSFUL_REQUEST;
		resultPair.second = getMessage;
	}

	return resultPair;
}

std::pair<int, serverMessage> GlobalServer::getMessage()
{
	std::pair<int, serverMessage> resultPair;

	if (!this->hasConnection())
	{
		resultPair.first = CONNECTION_ERROR;
		return resultPair;
	}
	
	std::pair<int, std::string> resultString = this->SocketManager::getMessage();

	if (resultString.first > 0)
	{
		std::pair<int, std::string> getPair = this->Protocol::makeGetString(resultString.second, resultString.first);

		std::string getString = cr->decrypt(getPair.second);

		serverMessage getMessage;

		get_request_body(getString, getMessage);

		resultPair.first = SUCCESSFUL_REQUEST;
		resultPair.second = getMessage;
	}

	return resultPair;
}

