#pragma once

#include "CommonTypes.h"

class IEventSender {
public:
    virtual void send_event(IEvent*) = 0;
};

class IEventListener {
public:
    virtual void get_event(IEvent*) = 0;
};

class IEventSubscriber {
public:
    virtual void subscribe(IEventListener*, EventTypes) = 0;
    virtual void unsubscribe(const IEventListener*) = 0;
};
