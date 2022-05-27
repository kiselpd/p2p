#include "UI.h"

Window::Window(unsigned int width, unsigned int height, int x, int y,  std::string path,  std::string fontpath, std::string mympath, std::string othmpath) {
	this->shape.x = width;
	this->shape.y = height;
	this->pos.x = x;
	this->pos.y = y;
	this->form = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), "spears", sf::Style::Close));
    this->form->setSize(shape);
	this->form->setPosition(pos);
	this->form->setTitle("spears");
	if (path != "none") {
        this->fillpicture = std::unique_ptr<sf::Texture>(new sf::Texture());
        this->fillpicture->loadFromFile(path);
    }
    else{
        this->fillpicture = NULL;
    }
    this->inter_now = std::unique_ptr<UsersInterface>(new UsersInterface(this->shape.x, this->shape.y, fontpath));
    this->inter_launch = std::unique_ptr<UsersInterface>(new UsersInterface(this->shape.x, this->shape.y, fontpath));
    this->inter_wait = std::unique_ptr<UsersInterface>(new UsersInterface(this->shape.x, this->shape.y, fontpath));
    this->messages = std::unique_ptr<MessageList>(new MessageList(width - 200, height - 100, 255, 255, 255, 240, 240, 255, fontpath));
}

void Window::init(unsigned int width, unsigned int height, int x, int y,  std::string path){
    this->shape.x = width;
	this->shape.y = height;
	this->pos.x = x;
	this->pos.y = y;
	this->form->setSize(shape);
	this->form->setPosition(pos);
	if (fillpicture == NULL) {
        this->fillpicture = std::unique_ptr<sf::Texture>(new sf::Texture());
        this->fillpicture->loadFromFile(path);
    }
    this->inter_now->setShape(width, height);
}

void Window::redraw() {
	this->form->clear();
	if (fillpicture != NULL){
        sf::Sprite tempspr(*fillpicture);
        tempspr.setTextureRect(sf::IntRect(0, 0, this->shape.x, this->shape.y));
        this->form->draw(sf::Sprite(*fillpicture));
	}
	this->messages->redraw(this->form.get());
	switch ( workint ){
    case 0:
        this->inter_launch->redraw(this->form.get());
        break;
    case 1:
        this->inter_wait->redraw(this->form.get());
        break;
    case 2:
        this->inter_now->redraw(this->form.get());
        break;
	}
	this->form->display();
}

int Window::open(IEventSender *ev_s) {
    this->form->display();
	this->redraw();
	this->workint = 0;
	this->launch_work(ev_s);
	this->workint = 1;
	this->wait_work(ev_s);
	this->workint = 2;
	this->work(ev_s);
	return 0;
}

void Window::close(IEventSender *ev_s) {
	this->form->close();
	//IEvent ex = new ExitEvent;
	//ev_s->send_event(ex);
}

Window::~Window() {
}

