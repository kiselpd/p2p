#include "UI.h"
#include <iostream>

int empty_func(){
    return 0;
}

int send_message(){
    return Request::MESSAGE;
}

int autorize(){
    return Request::AUTHORIZATION;
}

int reg(){
    return Request::REGISTRATION;
}

int search_friend(){
    return Request::SEARCH_FRIEND;
}

int accept(){
    return RequestStatus::SUCCESSFUL_REQUEST;
}

int not_accept(){
    return RequestStatus::NOT_ACCEPT_FRIEND;
}

//temporary part. Will be replaced later
UI::UI() : UI::UI(1366, 768, 10, 10, "static/img/back.png", "static/ARIALUNI.TTF", "static/img/cloud.png", "static/img/cloud_g.png"){
    sf::Color white = sf::Color::White;
    sf::Color blue = sf::Color(100, 100, 255);
    int tfsize = FONT_SIZE + 2 * BORDER;
    this->main_window->inter_launch->addField(1366 / 2 - 500/2, 768/2 - 2*tfsize - BORDER , 500, FONT_SIZE, 10, &white, false, L"Логин:                                       ");
    this->main_window->inter_launch->addField(1366 / 2 - 500/2, 768/2 - tfsize - BORDER, 500, FONT_SIZE, 40, &blue, true, "");

    this->main_window->inter_launch->addField(1366 / 2 - 500/2, 768/2  , 500, FONT_SIZE, 40, &white, false, L"Пароль:                                                 ");
    this->main_window->inter_launch->addField(1366 / 2 - 500/2, 768/2 + tfsize, 500, FONT_SIZE, 40, &blue, true, "");

    this->main_window->inter_launch->addField(1366 / 2 - 250/2, 768/2 + 2 * tfsize + BORDER, 250, FONT_SIZE, 40, &white, false, L"Авторизация:                                ");
    this->main_window->inter_launch->addField(1366 / 2 - 250/2, 768/2 + 3 * tfsize + BORDER, 250, FONT_SIZE, 40, &blue, false, L"Регистрация: ");

    this->main_window->inter_launch->addButton(1366 / 2 + 250/2 - 2 * BORDER, 768/2 + 2 * tfsize + BORDER /2, BUTTON_SIZE, BUTTON_SIZE, "static/img/rocket.png", &autorize, true);
    this->main_window->inter_launch->addButton(1366 / 2 + 250/2 - 2 * BORDER, 768/2 + 3 * tfsize+ BORDER /2, BUTTON_SIZE, BUTTON_SIZE, "static/img/ufo.png", &reg, true);
//Ожидание начала беседы
    this->main_window->inter_wait->addField(1366 / 2 - 500/2, 768/2 - 2*tfsize - BORDER , 500, FONT_SIZE, 10, &blue, false, L"Логин собеседника:                                       ");
    this->main_window->inter_wait->addField(1366 / 2 - 500/2, 768/2 - tfsize - BORDER, 500, FONT_SIZE, 40, &white, true, "");
    this->main_window->inter_wait->addButton(1366 / 2 + 500/2 - 2 * BORDER, 768/2 - tfsize - BORDER * 3 / 2, BUTTON_SIZE, BUTTON_SIZE, "static/img/glass.png", &search_friend, true);
//заранее невидимые кнопки для ответа на запрос на переписку
    this->main_window->inter_wait->addButton(1366 / 2 + BUTTON_SIZE / 2 + BORDER / 2, 768/2 +tfsize, BUTTON_SIZE, BUTTON_SIZE, "static/img/x.png", &autorize, false);
    this->main_window->inter_wait->addButton(1366 / 2 - BUTTON_SIZE / 2 - BORDER / 2, 768/2  +tfsize, BUTTON_SIZE, BUTTON_SIZE, "static/img/V.png", &reg, false);
//основной интерфейс чата - поле ввода и кнопка отправки сообщения
    this->main_window->inter_now->addField(BUTTON_SIZE, -BUTTON_SIZE, 1150, 40, 75, &white, true, "");
    this->main_window->inter_now->addButton(-BUTTON_SIZE, -BUTTON_SIZE, BUTTON_SIZE, BUTTON_SIZE, "static/img/rocket.png", &send_message, true);
}

void UI::setEventSender(EventManager* ev_s){
    this->event_sender = ev_s;
}

void UI::setEventSubscriber(EventManager* ev_h){
    this->event_subscriber = ev_h;
    ev_h->subscribe(this, EXIT_EXCEPTION);
    ev_h->subscribe(this, GET_MESSAGE);
}

void UI::run(){
    this->main_window->open(this->event_sender);
}

UI::UI(unsigned int width, unsigned int height, int x, int y,  std::string path, std::string fontp, std::string mmp, std::string omp) {
    this->main_window = std::unique_ptr<Window>(new Window(width, height, x, y, path, fontp, mmp, omp));
}

UI::~UI() {

}

void UI::get_event(IEvent* new_event) {
    this->main_window->eventQueue.push(new_event);
}

int cut_message(sf::String &str, size_t maxlen){
    size_t i = 0;
    int strings = 1;
    while (i + maxlen < str.getSize()){
        int j = maxlen;
        while (( j >= 0 ) && (str[i + j] != L'\n')){
            if (str[i + j] == ' '){
                str[i + j] = L'\n';
                ++strings;
            }
            else{
                --j;
            }
        }
        if (j == -1){
            str.insert(i + maxlen, "\n");
            ++strings;
            i += maxlen + 1;
        }
        else{
            i += j + 1;
        }
    }
    return strings;
}
