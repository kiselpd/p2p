#include "UI.h"
#include <iostream>
void Window::launch_work(IEventSender *ev_s){
    Message m;
    sf::Text error_txt;
    error_txt.setFillColor(sf::Color(255, 0, 0));
    error_txt.setPosition(0, 0);
    error_txt.setString(L"");
    sf::Event event;
    RequestStatus got_status;
    int mouse_res;
    int successful_launch = 0;
    while ((this->form->isOpen()) && ( successful_launch == 0 )){
        while (this->form->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->close(ev_s);
        }
        this->redraw();
        sf::sleep(sf::milliseconds(5));
        mouse_res = this->inter_launch->work(this->form.get(), event);

        switch (mouse_res){

            case Request::AUTHORIZATION: {
                m.text = this->inter_launch->take_message();
                m.status = SUCCESSFUL_REQUEST;
                m.type = AUTHORIZATION;
                IEvent *mev = new MessageEvent(EventTypes::SEND_MESSAGE, m);
                ev_s->send_event(mev);
            }
                break;

            case Request::REGISTRATION: {
                m.text = this->inter_launch->take_message();
                m.status = SUCCESSFUL_REQUEST;
                m.type = REGISTRATION;
                IEvent* mev = new MessageEvent(EventTypes::SEND_MESSAGE, m);
                ev_s->send_event(mev);
            }
                break;
         }

        if( !eventQueue.empty() ){
			IEvent *ev = eventQueue.front();
			if ( ev->type == GET_MESSAGE ){
                got_status = (dynamic_cast<MessageEvent*>(ev))->msg.status;
			}
			if ( ev->type ==  EXIT_EXCEPTION ){
                this->close(ev_s);
			}
			eventQueue.pop();
			switch ( got_status ){

            case RequestStatus::LOGIN_ERROR: {
                    if (m.type == REGISTRATION){
                        error_txt.setString(L"Такой логин уже существует");
                    }
                    if (m.type == AUTHORIZATION){
                        error_txt.setString(L"Логин не найден");
                    }
                }
                break;

            case RequestStatus::PASSWORD_ERROR: {
                    error_txt.setString(L"Неправильный пароль");
                }
                break;

            case RequestStatus::SUCCESSFUL_REQUEST: {
                    error_txt.setString(L"");
                    successful_launch = 1;
                }
                break;
            default:
                break;
            }
		}
    }
}

void Window::wait_work(IEventSender *ev_s){
    Message m;
    sf::Text error_txt;
    error_txt.setFillColor(sf::Color(255, 0, 0));
    error_txt.setPosition(0, 0);
    error_txt.setString(L"");
    sf::Event event;
    RequestStatus got_status;
    Request req_type;
    int mouse_res;
    int successful_launch = 0;
    int wait_answer = 0;
    int i_am_a_waiting_one_it_seems = 0;
    while ((this->form->isOpen()) && ( successful_launch == 0 )){
        while (this->form->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->close(ev_s);
        }

        this->redraw();
        sf::sleep(sf::milliseconds(5));
        mouse_res = this->inter_wait->work(this->form.get(), event);

        if ( mouse_res == Request::SEARCH_FRIEND ) {
            m.text = this->inter_wait->take_message();
            m.status = SUCCESSFUL_REQUEST;
            m.type = SEARCH_FRIEND;
            IEvent *mev = new MessageEvent(EventTypes::SEND_MESSAGE, m);
            ev_s->send_event(mev);
            i_am_a_waiting_one_it_seems = 1;
        }

        if( !eventQueue.empty() ){
            IEvent *ev = eventQueue.front();
            if ( ev->type == GET_MESSAGE ){
                got_status = (static_cast<MessageEvent*>(ev))->msg.status;
                req_type = (static_cast<MessageEvent*>(ev))->msg.type;
            }
            if ( ev->type ==  EXIT_EXCEPTION ){
                this->close(ev_s);
            }
            if (( req_type == GET_FRIEND ) && ( got_status == SUCCESSFUL_REQUEST )){
               sf::Color white = sf::Color::White;
                sf::String mes;
                mes = L"Пользователь ";
                mes += sf::String((static_cast<MessageEvent*>(ev))->msg.text) + sf::String(L" предлагает начать общение. Начать разговор?                                                                     ");
                this->inter_wait->addField(this->shape.x / 2 - 250, this->shape.y / 2, 500, FONT_SIZE, 40, &white, false, mes);
                this->inter_wait->buttons[1]->show();
                this->inter_wait->buttons[2]->show();
                wait_answer = 1;
            }

            eventQueue.pop();
            if ( i_am_a_waiting_one_it_seems ){
                switch ( got_status ){

                case RequestStatus::NOT_AUTHORIZED: {
                        error_txt.setString(L"Искомый пользователь не в сети");
                        i_am_a_waiting_one_it_seems = 0;
                    }
                    break;

                case RequestStatus::NOT_FOUND: {
                        error_txt.setString(L"Логин не найден");
                        i_am_a_waiting_one_it_seems = 0;
                        }
                    break;

                case RequestStatus::NOT_ACCEPT_FRIEND: {
                        error_txt.setString(L"В разговоре отказано");
                        i_am_a_waiting_one_it_seems = 0;
                    }
                    break;

                case RequestStatus::SUCCESSFUL_REQUEST: {
                        error_txt.setString(L"");
                        successful_launch = 1;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        if ( wait_answer ){
            if ( mouse_res == SUCCESSFUL_REQUEST ){
                successful_launch = 1;
                m.text = "";
                m.status = SUCCESSFUL_REQUEST;
                m.type = GET_FRIEND;
                IEvent *mev = new MessageEvent(EventTypes::SEND_MESSAGE, m);
                ev_s->send_event(mev);
            }

            if ( mouse_res == NOT_ACCEPT_FRIEND ){
                m.text = "";
                m.status = NOT_ACCEPT_FRIEND;
                m.type = GET_FRIEND;
                IEvent *mev = new MessageEvent(EventTypes::SEND_MESSAGE, m);
                ev_s->send_event(mev);
                this->inter_wait->tfields_count -= 1;
                this->inter_wait->tfields.pop_back();
                this->inter_wait->buttons[1]->hide();
                this->inter_wait->buttons[2]->hide();
                wait_answer = 0;
            }
        }
    }
}

void Window::work(IEventSender *ev_s){
    Message m;
    sf::Event event;
    RequestStatus got_status;
    Request req_type;
    int mouse_res;
 while (this->form->isOpen())
    {
        while (this->form->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->close(ev_s);
        }

        mouse_res = this->inter_now->work(this->form.get(), event);

        //колесо мыши
        if (event.type == sf::Event::MouseWheelScrolled){
            int new_step = event.mouseWheelScroll.delta;
           this->messages->step(2*new_step);
            new_step = 0;
           sf::sleep(sf::milliseconds(20));
        }

        this->redraw();
        sf::sleep(sf::milliseconds(5));

        switch (mouse_res){

            case Request::MESSAGE: {
                m.text = this->inter_now->take_message();
                m.status = SUCCESSFUL_REQUEST;
                //m.timestamp = ;
                m.type = MESSAGE;
                IEvent *mev = new MessageEvent(EventTypes::SEND_MESSAGE, m);
                ev_s->send_event(mev);
                this->messages->append(&m, ME);
            }
                break;
         }

       if( !eventQueue.empty() ){
			IEvent *ev = eventQueue.front();
			if ( ev->type == GET_MESSAGE ){
                got_status = (static_cast<MessageEvent*>(ev))->msg.status;
                req_type = (static_cast<MessageEvent*>(ev))->msg.type;
			}
			if ( ev->type == EXIT_EXCEPTION ){
                this->close(ev_s);
			}
			//пришло сообщение, нужно его добавить
			if (( got_status == SUCCESSFUL_REQUEST ) && ( req_type == MESSAGE )){
                m.text = (static_cast<MessageEvent*>(ev))->msg.text;
                m.status = SUCCESSFUL_REQUEST;
                //m.timestamp = ;
                this->messages->append(&m, YOU);
			}
			eventQueue.pop();
		}
    }
}
