#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <functional>
//
#include<iostream>

#include "Server.h"
#include "Connection.h"
#include "Crypto.h"

int WAIT_TIME = 1000;

void Server::get_event(IEvent *new_event) 
{
	this->eventQueue.push(new_event);
}

void Server::setEventSender(IEventSender* ev_h)
{
	this->eventSender = ev_h;
}

void Server::setEventSubscriber(IEventSubscriber* ev_h)
{
	this->eventSubscriber = ev_h;
}

Server::Server()
{
	cr.initialize();
	GlobalServer server(&cr);
	Client client(&cr);
}

int Server::processingRequestFromUI(IEvent* event)
{
	int statusMes;
	MessageEvent* message = dynamic_cast<MessageEvent*>(event);
	//cr.set_key(PUBLIC_KEY, server.publicKey);
	
	switch (message->msg.type)
	{
		case AUTHORIZATION:
		{
			std::cout << "tyt";
			int poz = (message->msg.text).find("\n");

			if(poz != (message->msg.text).npos)
				statusMes = server.sendAuthorizationRequest((message->msg.text).substr(0, poz), (message->msg.text).substr(poz + 1));
			break;
		}
		case REGISTRATION:
		{
			int poz = (message->msg.text).find("\n");

			if(poz != (message->msg.text).npos)
				statusMes = server.sendRegistrationRequest((message->msg.text).substr(0, poz), (message->msg.text).substr(poz + 1));
			break;
		}
		case SEARCH_FRIEND:
		{
			statusMes = server.sendSearchRequest(message->msg.text);
			
			if(statusMes == SUCCESSFUL_REQUEST)
				client.clientLogin = message->msg.text;
			break;
		}
		case GET_FRIEND:
		{
			if(message->msg.status == SUCCESSFUL_REQUEST)
			{
				client.clientLogin = message->msg.text;
				client.sendFriendshipRequest(SUCCESSFUL_REQUEST);
			}
			else
			{
				client.sendFriendshipRequest(NOT_ACCEPT_FRIEND);
				client.toDisconnect();
			}

			break;
		}
		case MESSAGE:
		{
			cr.set_key(PUBLIC_KEY, client.publicKey);
			statusMes = client.sendMessage(message->msg.text, message->msg.timestamp);
		}
		default:
			break;
	}

	return statusMes;
}

void Server::processingRequestFromServer(serverMessage serverMsg)
{
	Message msg;
	msg.type = serverMsg.type;

	switch (serverMsg.type)
	{
	case AUTHORIZATION:
	{
		if((serverMsg.text).size() > 1)
		{
			int poz = (serverMsg.text).find(":");

			if(poz != (serverMsg.text).npos)
			{
				server.myLogin = serverMsg.login;
				server.ip = (serverMsg.text).substr(0, poz);
				server.port = (serverMsg.text).substr(poz + 1);
				msg.status = SUCCESSFUL_REQUEST;
			}
			else
				msg.status = REQUEST_ERROR;
		}
		else
			msg.status = (RequestStatus)(std::stoi(serverMsg.text));

		break;
	}
	case REGISTRATION:
	{
		if((serverMsg.text).size() > 1)
		{
			int poz = (serverMsg.text).find(":");

			if(poz != (serverMsg.text).npos)
			{
				server.myLogin = serverMsg.login;
				server.ip = (serverMsg.text).substr(0, poz);
				server.port = (serverMsg.text).substr(poz + 1);
				msg.status = SUCCESSFUL_REQUEST;
			}
			else
				msg.status = REQUEST_ERROR;
		}
		else
			msg.status = (RequestStatus)(std::stoi(serverMsg.text));

		break;
	}
	case SEARCH_FRIEND:
	{
		if((serverMsg.text).size() > 1)
		{
			int poz1 = (serverMsg.text).find(':');
			int poz2 = (serverMsg.text).rfind(':');

			if((poz1 != (serverMsg.text).npos) && (poz2 != (serverMsg.text).npos) && (poz1 != poz2))
			{
				client.toConnect((serverMsg.text).substr(0, poz1), (serverMsg.text).substr(poz1 + 1, poz2 - poz1 - 1));
				client.publicKey = (serverMsg.text).substr(poz2 + 1);
				client.sendFriendshipRequest(SUCCESSFUL_REQUEST);
			}
			else
				msg.status = REQUEST_ERROR;
		}
		else
		{
			(client.clientLogin).clear();
			msg.status = (RequestStatus)(std::stoi(serverMsg.text));
		}
		
		break;
	}
	case SEND_PUBLIC_KEY:
	{
		server.publicKey = serverMsg.text;

		break;
	}
	case GET_ADDRESS:
	{	
		if((serverMsg.text).size() > 1)
		{
			int poz = (serverMsg.text).find(":");

			if(poz != (serverMsg.text).npos)
			{
				server.ip = (serverMsg.text).substr(0, poz);
				server.port = (serverMsg.text).substr(poz + 1);
			}
			else
				msg.status = REQUEST_ERROR;
		}
		else
			msg.status = (RequestStatus)(std::stoi(serverMsg.text));
		
		break;
	}
	case GET_FRIEND:
	{
		break;
	} 
	default:

		break;
	}
	
	IEvent *r = new MessageEvent(GET_MESSAGE, msg);
	if((msg.type != SEND_PUBLIC_KEY) && (msg.type != GET_ADDRESS))
		eventSender->send_event(r);
}

void Server::processingMessageFromClient(clientMessage msg)
{
	Message clientMes;
	if((client.clientLogin).empty())
	{

	}

	if((msg.text).find("/!@#$%^&*CoNnEcT*&^%$#@!/") != (msg.text).npos)
	{
		clientMes.type = GET_FRIEND;
		clientMes.status = SUCCESSFUL_REQUEST;
		clientMes.timestamp = msg.time;
	}
	else if((msg.text).find("/!@#$%^&*No_CoNnEcT*&^%$#@!/") != (msg.text).npos)
	{
		clientMes.type = GET_FRIEND;
		clientMes.status = NOT_ACCEPT_FRIEND;
		clientMes.timestamp = msg.time;
	}
	else
	{
		clientMes.type = MESSAGE;
		clientMes.status = SUCCESSFUL_REQUEST;
		clientMes.text = msg.text;
		clientMes.timestamp = msg.time;
	}
	
	IEvent *r = new MessageEvent(GET_MESSAGE, clientMes);
	eventSender->send_event(r);
}


int Server::run()
{
	std::string globalAddress = "ovz2.webstersx.pxlzp.vps.myjino.ru";
	std::string globalPort = "49332";
	eventSubscriber->subscribe(this, SEND_MESSAGE);
	
	auto start = clock();
	int serverConnect;

	while(true)
	{
		serverConnect = server.toConnect(globalAddress, globalPort);

		if(serverConnect == IS_DONE)
			break;
		if(start + WAIT_TIME < clock())
            return serverConnect; 
	}
	serverConnect = server.sendPublicKey(cr.get_key(PUBLIC_KEY));

	while(true)
	{	
		if(!(server.ip).empty() && !(server.port).empty() && (client.clientLogin).empty())
			std::thread td(std::bind(&Server::listenClient, this));

		if(!eventQueue.empty())
		{	
			int statusEvent;
			statusEvent = this->processingRequestFromUI(eventQueue.front());
			eventQueue.pop();
		}

		if((server.hasConnection()) && (!(server.myLogin).empty()))
		{
			std::pair<int, serverMessage> mes = server.getMessage();
			if(mes.first > 0)
				this->processingRequestFromServer(mes.second);
		}

		if(client.hasConnection())
		{	
			std::pair<int, clientMessage> mes = client.getMessage();
			if(mes.first > 0)
				this->processingMessageFromClient(mes.second);
		}
	}

	return this->exit();
	

}

int Server::exit()
{	
	int statusExit;

	if (client.hasConnection())
		statusExit = client.toDisconnect();

	if (server.hasConnection())
		statusExit = server.toDisconnect();

	return statusExit;
}

void Server::listenClient()
{	
	std::cout<<"thread";
	socklen_t listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (listenSocket < 0)
		return;

	const char* ip_char = (ip).c_str();

	sockaddr_in serv;
	hostent* hp = gethostbyname(ip_char);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(std::stoi(port));
	memcpy(&serv.sin_addr, hp->h_addr, hp->h_length);

	int result = bind(listenSocket, (sockaddr*)&serv, sizeof(serv));

	if (result == -1)
	{
		result = close(listenSocket);
		return;
	}

	if (listen(listenSocket, 1) == 0)
	{
		result = close(listenSocket);
		return;
	}

	socklen_t clientSocket = accept(listenSocket, NULL, NULL);

	if (clientSocket <= 0)
	{
		result = close(listenSocket);
		return;
	}
	else
		client.getSocket(clientSocket);

}
