#include "UI.h"

VisualElem::VisualElem() {

	pos[0] = 0;
    pos[1] = 0;
    shape[0] = 0;
    shape[1] = 0;
}

VisualElem::VisualElem(int x, int y, int w, int h) {
    	pos[0] = x;
    	pos[1] = y;
    	shape[0] = w;
    	shape[1] = h;
}

VisualElem::VisualElem(int x, int y, int w, int h, std::string path) :  VisualElem::VisualElem(x, y, w, h) {
    	this->img = std::unique_ptr<sf::Texture> (new sf::Texture());
    	this->img->loadFromFile(path);
        this->spr = std::unique_ptr<sf::Sprite> (new sf::Sprite(*(this->img)));
        sf::Vector2u old_size = this->img->getSize();
    	this->spr->setPosition(x, y);
    	this->spr->setTextureRect(sf::IntRect(0, 0, old_size.x, old_size.y));
        this->spr->setScale(1.0 * w / old_size.x, 1.0 * h / old_size.y );
}

VisualElem::~VisualElem() {

}

void VisualElem::init(int x, int y, int w, int h){
    	pos[0] = x;
    	pos[1] = y;
    	shape[0] = w;
    	shape[1] = h;
}

bool VisualElem::isIntersected(int x, int y){
    return (*(this->spr)).getGlobalBounds().contains(x, y);
}

void VisualElem::init(int x, int y, int w, int h, std::string path) {
        this->init(x, y, w, h);
        this->img->loadFromFile(path);
        this->spr->setTexture(*(this->img));
        sf::Vector2u old_size = this->img->getSize();
    	this->spr->setPosition(x, y);
    	this->spr->setTextureRect(sf::IntRect(0, 0, old_size.x, old_size.y));
        this->spr->setScale(w / old_size.x, h / old_size.y);


    }

void VisualElem::hide(){
    	this->isVisible = false;
    }

void VisualElem::show(){
    	this->isVisible = true;
    }

void VisualElem::redraw(sf::RenderWindow *win){
    if (this->isVisible)
        win->draw(*(this->spr));
}


int Button::press() {
	return func();
}

void Button::hide(){
    this->img->hide();
}

void Button::show(){
    this->img->show();
}

Button::Button(int x, int y, int w, int h, std::string path, int(*action)()) {
    this->img = std::unique_ptr<VisualElem> (new VisualElem(x, y, w, h, path));
    func = action;
}

Button::Button() {
    this->img = std::unique_ptr<VisualElem> (new VisualElem());
}

void Button::redraw(sf::RenderWindow *win) {
   this->img->redraw(win);
}

bool Button::is_intersected(int x, int y){
    if (this->img->isVisible)
        return this->img->isIntersected(x, y);
    else
        return false;
}

Button::~Button() {

}
