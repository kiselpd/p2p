#include "EventManager.h"

void EventManager::send_event(IEvent *event) {
    for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
        if(it->second == event->type)
            it->first->get_event(event);
    }
}

void EventManager::subscribe(IEventListener* listener, EventTypes event_type) {
    for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
        if(it->first == listener && it->second == event_type)
            return;
    }

    subscribers.push_back(std::make_pair(listener, event_type));
}

void EventManager::unsubscribe(const IEventListener* listener) {
    for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
        if(it->first == listener)
            subscribers.erase(it);
    }
}
