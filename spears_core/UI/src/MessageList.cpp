#include "UI.h"

ShowMessage::ShowMessage(int x, int y, int w, int h, sf::Color *color, sf::Font *font, Message *new_message){
    this->pos[0] = x;
    this->pos[1] = y;

	this->content = std::unique_ptr<Message>(new Message);
	this->content->text = new_message->text;
	this->content->status = new_message->status;
	this->content->timestamp = new_message->timestamp;
	this->content->type = new_message->type;
	this->text = std::unique_ptr<TextField>(new TextField(x, 50, w, h, content->text.length(), font, FONT_SIZE, color));
	this->text->setText(sf::String(this->content->text));
}

ShowMessage::~ShowMessage(){
}

void ShowMessage::draw(sf::RenderWindow * win, int position){
    this->text->changepos(this->pos[1] + position);
    this->text->redraw(win);
}

int ShowMessage::get_y(){
    return this->pos[1];
}

MessageList::MessageList(int w, int h, int my_r, int my_g, int my_b, int o_r, int o_g, int o_b, std::string font_path) {
    this->my_message_color = std::unique_ptr<sf::Color>(new sf::Color(my_r, my_g, my_b));
	this->others_message_color = std::unique_ptr<sf::Color>(new sf::Color(o_r, o_g, o_b));
	this->font = std::unique_ptr<sf::Font>(new sf::Font());
	this->font->loadFromFile(font_path);
	this->width = w;
	this->height = h;
	this->length = 0;
	this->position = 0;
}

MessageList::~MessageList() {

}


void MessageList::append(Message *new_one, Sender sender) {
    if (new_one->text.length() < 1){
        return;
    }
    //посчитаем, как расположить текст. Под одно сообщение - поллиста по ширине.
    int w = (this->width) / 2 - BUTTON_SIZE;
    //определим местоположение и вид "облачка" с текстом в зависимоти от отправителя сообщения
    int x, y;
    sf::Color *c;
    if (sender == ME){
        x = width - w;
        c = this->my_message_color.get();
    }
    else{
        x = BUTTON_SIZE;
        c = this->others_message_color.get();
    }
    //определим размеры облачка с текстом:
    //координата y
    y = this->length;
    sf::String newst = new_one->text;
    int h = cut_message(newst, w / FONT_SIZE * 3 / 2) * FONT_SIZE;
    this->length += h + FONT_SIZE + BORDER;
    this->position = this->length;
    this->messages.push_back(std::unique_ptr<ShowMessage>(new ShowMessage(x, y, w, h, c, this->font.get(), new_one)));
    this->messages_count++;
}

void MessageList::redraw(sf::RenderWindow * win){
    for (int i = 0; i<this->messages_count; ++i){
        if ((this->messages[i]->get_y() >= this->position - this->height) && (this->messages[i]->get_y() <= this->position)) {
            this->messages[i]->draw(win, this->height - this->position);
        }
    }
}

int MessageList::step(int direction) {
    this->position += direction;
    if ( this->position > this->length ){
        this->position=this->length;
    }
    if ( this->position < 0 ){
        this->position=0;
    }
	return 0;
}
