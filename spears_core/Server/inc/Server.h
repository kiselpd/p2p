#pragma once
#include <string>
#include <queue>
#include "Connection.h"
#include "CommonTypes.h"
#include "EventInterface.h"
#include "EventManager.h"
#include "Crypto.h"

class Server : public IEventListener
{
public:
//private:
	GlobalServer server;
	Client client;
	Crypto cr;
	std::queue<IEvent*> eventQueue;
	IEventSender* eventSender;
	IEventSubscriber* eventSubscriber;
	std::string ip;
	std::string port;

//private:
	void setEventSender(IEventSender* a);
	void setEventSubscriber(IEventSubscriber* a);
	void get_event(IEvent*) override;
	int processingRequestFromUI(IEvent* event);
	void processingRequestFromServer(serverMessage serverMsg);
	void processingMessageFromClient(clientMessage msg);
	Message addClient(std::string ip, std::string port);
	void listenClient();

	int exit();

//public:
	Server();
	int run();
};
