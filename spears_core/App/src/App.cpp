#include "App.h"

#include <thread>
#include <mutex>

void App::run() {

    std::thread ui(&UI::run, graphics);
    std::thread server(&Server::run, srv);

    ui.join();
    server.join();
}

void App::initialize() {

    event_handler = new EventManager;
	graphics = new UI;
	srv = new Server;

    graphics->setEventSender(event_handler);
    graphics->setEventSubscriber(event_handler);

    srv->setEventSender(event_handler);
    srv->setEventSubscriber(event_handler);

    event_handler->subscribe(this, CATCH_EXCEPTION);
    event_handler->subscribe(this, APP_EXIT);

    run();
}

void App::get_event(IEvent* e) {
/*
    if(e->type == APP_EXIT) {
        ExitEvent* evt = dynamic_cast<ExitEvent*>(e);
        event_handler->send_event(ExitEvent(APP_EXIT, evt));
    }
    else if (e->type == CATCH_EXCEPTION) {
        ExceptionEvent* evt = dynamic_cast<ExceptionEvent*>(e);
        event_handler->send_event(ExceptionEvent(EXIT_EXCEPTION, evt->e));
    }
    */
}

App::~App() {
    delete event_handler;
	delete graphics;
	delete srv;
}
