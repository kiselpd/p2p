#pragma once

#include "EventManager.h"
#include "UI.h"
#include "Server.h"

class App : public IEventListener {

public:
    void initialize();
    
    ~App();
        
private:
    UI* graphics;
    Server* srv;
    EventManager* event_handler;
    
    void run();
    void get_event(IEvent*);
};
