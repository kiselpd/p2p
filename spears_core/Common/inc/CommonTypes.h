#pragma once

#include <string>
#include <vector>

enum EventTypes { GET_MESSAGE, SEND_MESSAGE, CATCH_EXCEPTION, EXIT_EXCEPTION, APP_EXIT };
enum Request { REGISTRATION, AUTHORIZATION, SEARCH_FRIEND, SEND_PUBLIC_KEY, GET_FRIEND, MESSAGE, GET_ADDRESS };
enum RequestStatus { REQUEST_ERROR, LOGIN_ERROR, PASSWORD_ERROR, NOT_AUTHORIZED, NOT_FOUND, NOT_ACCEPT_FRIEND, SUCCESSFUL_REQUEST };

class IEvent {
public:
    IEvent(EventTypes type) : type(type) { };
    EventTypes type;
    virtual ~IEvent() = default;
};

struct Message {
    RequestStatus status;
    Request type;
    std::string timestamp;
    std::string text;
};

class ExceptionEvent : public IEvent{

public:
    ExceptionEvent(EventTypes type, std::exception e) : IEvent(type), e(e) { };
    std::exception e;
    virtual ~ExceptionEvent() = default;
};

class MessageEvent : public IEvent{

public:
    MessageEvent(EventTypes type, Message msg) : IEvent(type), msg(msg) { };
    Message msg;
    virtual ~MessageEvent() = default;
};

class ExitEvent : public IEvent{

public:
    ExitEvent() : IEvent(APP_EXIT) { };
    virtual ~ExitEvent() = default;
};
