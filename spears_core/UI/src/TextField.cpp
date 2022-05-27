#include "UI.h"
#include <iostream>
InputField::InputField(int x, int y, int w, int h, int maxlen, int maxstrlen, sf::Font *font,  unsigned int char_size, sf::Color *color) : TextField(x, y, w, h, maxlen, font, char_size, color) {
    this->max_string_length = maxstrlen;
}

InputField::InputField() : TextField(){

}

InputField::~InputField() {


}

sf::String InputField::take_text(){
    sf::String res = this->content_real;
    this->content = u8"";
    this->content_real = u8"";
    return res;
}

TextField::TextField(int x, int y, int w, int h, int maxlen, sf::Font *font,  unsigned int char_size, sf::Color *color) {
    int w1, h1;
    h1 = h;
    w1 = w;
    this->img = std::unique_ptr<TextBG> (new TextBG(x, y, w1, h1, color));
    this->content = u8"";
    this->content_real = u8"";
    this->text = std::unique_ptr<sf::Text> (new sf::Text(u8"", *font, char_size));
    this->text->setString(u8"");
    this->text->setFillColor(sf::Color(0, 0, 0));
    this->text->setPosition(x, y);
    this->max_content_length = maxlen;
    this->pos[0] = x;
    this->pos[1] = y;
}

TextField::TextField() {
    this->img = std::unique_ptr<TextBG> (new TextBG());
    this->text = std::unique_ptr<sf::Text> (new sf::Text());
}

void TextField::setText(sf::String str){
    //разобьём сообщение, чтобы оно помещалось по ширине
    int w = this->img->giveWidth();
    int char_per_string = w / FONT_SIZE * 3 / 2;
    this->content_real = str;
    this->content = str;
    int strings_count = cut_message(this->content, char_per_string);
    //тогда высота "облачка" для сообщения должна быть:
    int h = strings_count * FONT_SIZE;
    this->img->init(this->pos[0], this->pos[1], w, h);
}

TextField::~TextField() {

}

void TextField::changepos(int y){
    this->img->set_pos(y);
    this->text->setPosition(this->pos[0], y-5);
}

void InputField::work(sf::Event event){
    static sf::Uint32 old_letter = 0xD;
    if (( old_letter != 0xD ) && ( event.text.unicode == old_letter )){
        sf::sleep(sf::milliseconds(100));
        old_letter = 0xD;
    }
    else{
        switch ( event.text.unicode ){
            //Ввод
            case 0xD:
                //this->content += L'\n' ;
                this->isActive = false;
                break ;
            //Стереть
            case 0x8:
                if ( !(this->content_real).isEmpty() ) {
                    this->content.erase(this->content.getSize()-1) ;
                    this->content_real.erase(this->content_real.getSize()-1) ;
                }
                break;
            //ctrl+C
            case 0x3:{
                sf::Clipboard::setString(this->content_real);
            }
                break;
            //ctrl+V
            case 0x16:{
                this->content_real += sf::Clipboard::getString();
                this->content += sf::Clipboard::getString();
                if (this->content.getSize() >= this->max_content_length - 1) {
                    this->content.erase(this->content.getSize()-1) ;
                    this->content_real.erase(this->content_real.getSize()-1) ;
                }
            }
                break;
            //Буквы
            default :
            {
                if (this->content.getSize() < this->max_content_length){
                    if (this->content.getSize() >= this->max_string_length - 1){
                       // this->content += L'\n' ;
                       // this->img->set_pos(this->pos + FONT_SIZE);
                       // this->img->init()
                    }
                    this->content += static_cast<wchar_t>(event.text.unicode);
                    this->content_real += static_cast<wchar_t>(event.text.unicode);
                }
            }
        }
        this->text->setString(this->content);
        old_letter = event.text.unicode;
    }
}

bool TextField::is_intersected(int x, int y){
    return this->img->isIntersected(x, y);
}

void TextField::redraw(sf::RenderWindow *win) {
    this->text->setString(this->content);
    this->img->redraw(win);
    win->draw(*this->text.get());
}

TextBG::TextBG(){

    this->rect_h = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(shape_h));
    this->rect_w = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(shape_w));
}

TextBG::TextBG(int x, int y, int w, int h){
    this->pos[0] = x;
    this->pos[1] = y;
    this->shape_h.x = w;
    this->shape_h.y = h + 2 * BORDER;
    this->shape_w.x = w+ 2 * BORDER;
    this->shape_w.y = h;
    this->rect_h = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(shape_h));
    this->rect_w = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(shape_w));
    this->shape.x = w;
    this->shape.y = h;
    for (int i = 0; i<4; i++){
        this->circles[i] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(BORDER));
    }
    this->set_pos(y);
}

TextBG::TextBG(int x, int y, int w, int h, sf::Color *color) : TextBG::TextBG(x, y, w, h) {
    this->rect_h->setFillColor(*color);
    this->rect_w->setFillColor(*color);
    for (int i = 0; i<4; i++){
        this->circles[i]->setFillColor(*color);
    }
}

int TextBG::giveWidth(){
    return this->shape.x;
}

TextBG::~TextBG(){

}

void TextBG::init(int x, int y, int w, int h){
    this->pos[0] = x;
    this->pos[1] = y;
    this->shape_h.x = w;
    this->shape_h.y = h + 2*BORDER;
    this->shape_w.x = w+ 2*BORDER;
    this->shape_w.y = h;
    this->shape.x = w;
    this->shape.y = h;
    this->set_pos(y);
    this->rect_h->setSize(shape_h);
    this->rect_w->setSize(shape_w);

}

void TextBG::init(int x, int y, int w, int h, sf::Color *color){
    this->init(x, y, w, h);
    this->rect_h->setFillColor(*color);
    this->rect_w->setFillColor(*color);
    for (int i = 0; i<4; i++){
        this->circles[i]->setFillColor(*color);
    }
}

void TextBG::set_pos(int y){
    this->pos[1] = y;
    this->rect_h->setPosition(pos[0], pos[1]-BORDER);
    this->rect_w->setPosition(pos[0]-BORDER, pos[1]);
    circles[0]->setPosition(this->pos[0] - BORDER, this->pos[1] - BORDER);
    circles[1]->setPosition(this->pos[0] + this->shape.x - BORDER, this->pos[1] - BORDER);
    circles[2]->setPosition(this->pos[0] - BORDER, this->pos[1] + this->shape.y - BORDER);
    circles[3]->setPosition(this->pos[0] + this->shape.x - BORDER, this->pos[1] + this->shape.y - BORDER);
}

void TextBG::redraw(sf::RenderWindow *win){
    win->draw(*(this->rect_h));
    win->draw(*(this->rect_w));
    for (int i = 0; i<4; i++){
        win->draw(*(this->circles[i]));
    }
}

bool TextBG::isIntersected(int x, int y){
    return ((this->rect_h->getGlobalBounds().contains(x, y)) || (this->rect_w->getGlobalBounds().contains(x, y)));
}
