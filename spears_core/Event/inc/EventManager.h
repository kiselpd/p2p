#pragma once

#include <vector>
#include <utility>

#include "EventInterface.h"


class EventManager : public IEventSubscriber, public IEventSender {
public:
    void send_event(IEvent*);
    void subscribe(IEventListener*, EventTypes);
    void unsubscribe(const IEventListener*);

private:
    std::vector<std::pair<IEventListener*, EventTypes>> subscribers;
};
