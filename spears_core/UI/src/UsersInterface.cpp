#include "UI.h"
/*
template<typename T>
void UsersInterface::make_array(std::vector<std::unique_ptr<T>> arr, int count) {
	for (int i = 0; i < count; ++i) {
        arr.push_back(std::unique_ptr<T>(new T()));
	}
}
*/
/*
template<typename T>
void UsersInterface::redraw_array(std::vector<T*> arr, int count, sf::RenderWindow *win) {
	for (int i = 0; i < count; ++i) {
        win->draw(sf::Sprite(arr[i]->img->img));
	}
}
*/
UsersInterface::UsersInterface(int w, int h, std::string fontpath) {
    this->width = w;
    this->height = h;
	fields_count = 0;
	tfields_count = 0;
	buttons_count = 0;
    this->font = std::unique_ptr<sf::Font>(new sf::Font);
    this->font->loadFromFile(fontpath);
}

void UsersInterface::setShape(int w, int h){
    this->width = w;
    this->height = h;
}

void UsersInterface::addButton(int x, int y, int w, int h, std::string path, int(*action)(), bool visible){
    this->buttons_count++;
    int x1 = x;
    int y1 = y;
    if (x < 0){
        x1 = this->width + x - w;
    }

    if (y < 0){
        y1 = this->height + y - h;
    }

    this->buttons.push_back(std::unique_ptr<Button>(new Button(x1, y1, w, h, path, action)));
    if ( !visible ){
        this->buttons[buttons_count-1]->hide();
    }
}

void UsersInterface::addField(int x, int y, int w, int h, int maxlen, sf::Color *color, bool isinput, sf::String text){
    int x1 = x;
    int y1 = y;
    if (x < 0){
        x1 = this->width + x - w;
    }
    if (y < 0){
        y1 = this->height + y - h;
    }
    if ( isinput ) {
        this->fields.push_back(std::unique_ptr<InputField>(new InputField(x1, y1, w, h, maxlen, maxlen / 4, this->font.get(), FONT_SIZE, color)));
        this->fields_count++;
    }
    else {
        this->tfields_count++;
        this->tfields.push_back(std::unique_ptr<TextField>(new TextField(x1, y1, w, h, maxlen, this->font.get(), FONT_SIZE, color)));
        this->tfields[tfields_count-1]->setText(text);
    }
}

void UsersInterface::redraw(sf::RenderWindow * win){
    //redraw_array<InputField>(this->fields, this->fields_count, win);
    //redraw_array<Button>(this->buttons, this->buttons_count, win);
    for (int i = 0; i < tfields_count; ++i) {
        tfields[i]->redraw(win);
	}

	for (int i = 0; i < fields_count; ++i) {
        fields[i]->redraw(win);
	}

	for (int i = 0; i < buttons_count; ++i) {
        buttons[i]->redraw(win);
	}
}

sf::String UsersInterface::take_message(){
    sf::String res = L"";
    for (int i = 0; i < this->fields_count; ++i){
        res += this->fields[i]->take_text() + L"\n";
    }
    res.erase(res.getSize() - 1);
    return res;
}

int UsersInterface::work(sf::RenderWindow *win, sf::Event event){
    int mouse_res = -10;
        //На щелчок мыши
    if (event.type == sf::Event::MouseButtonPressed)
    {
        //координаты мыши, где произошёл щелчок
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(*win);
        //если щелчок пришёлся на кнопку:
        for (int i=0; i<this->buttons_count; ++i){
            if (this->buttons[i]->is_intersected(mouse_pos.x, mouse_pos.y)){
                mouse_res = this->buttons[i]->press();
                sf::sleep(sf::milliseconds(100));
            }
        }
        //если щелчок пришёлся на поле ввода
        for (int i=0; i<this->fields_count; ++i){
            if (this->fields[i]->is_intersected(mouse_pos.x, mouse_pos.y)){
                this->fields[i]->isActive = true;
            }
            else {
                this->fields[i]->isActive = false;
            }
        }
    }

    //вводится текст? надо проверить, не вводятся ли данные
    if (event.type == sf::Event::TextEntered){
        for (int i=0; i<this->fields_count; ++i){
            if (this->fields[i]->isActive){
                this->fields[i]->work(event);
            }
        }
    }
    this->redraw(win);
    return mouse_res;
}

UsersInterface::~UsersInterface() {

}
