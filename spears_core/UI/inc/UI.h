#pragma once

#include <memory>
#include "CommonTypes.h"
#include "EventInterface.h"
#include "EventManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <cctype>
#include <queue>

const int FONT_SIZE = 22;

const int BUTTON_SIZE = 50;

const int BORDER = 20;

enum Sender { YOU, ME };

class TextBG {
private:
    std::unique_ptr<sf::RectangleShape> rect_h;
    std::unique_ptr<sf::RectangleShape> rect_w;
    std::shared_ptr<sf::Color> color;
    sf::Vector2f shape_h;
    sf::Vector2f shape_w;
    sf::Vector2f shape;
    std::shared_ptr<sf::CircleShape> circles[4];
public:
    int pos[2];

    void set_pos(int);

	TextBG(int, int, int, int);

    TextBG();

	TextBG(int, int, int, int, sf::Color *);

	~TextBG();

    void init(int, int, int, int);

    void init(int, int, int, int, sf::Color *);

    void redraw(sf::RenderWindow *);

    bool isIntersected(int, int);

    int giveWidth();
};

class VisualElem {
private:
    std::unique_ptr<sf::Texture> img;
    std::unique_ptr<sf::Sprite> spr;
    int pos[2];
    int shape[2];
public:

    bool isVisible = true;

	VisualElem();

	VisualElem(int, int, int, int);

	VisualElem(int, int, int, int, std::string);

	~VisualElem();

    void init(int, int, int, int);

    void init(int, int, int, int, std::string);

    void hide();

    void show();

    void redraw(sf::RenderWindow *);

    bool isIntersected(int, int);
};

class Button {
private:
	std::unique_ptr<VisualElem> img;
	int (*func)() = NULL;
public:

	Button(int, int, int, int, std::string, int(*)());

    Button();

	~Button();

	int press();

    void hide();

    void show();

	void redraw(sf::RenderWindow *);

	bool is_intersected(int, int);
};

class TextField {
protected:
    int pos[2];
	std::unique_ptr<TextBG> img;
	std::unique_ptr<sf::Text> text;
	sf::String content;
	sf::String content_real;
	std::size_t max_content_length;
public:

    TextField();

    void changepos(int );

	TextField(int, int, int, int, int, sf::Font *, unsigned int, sf::Color *color);

    void setText(sf::String);

	~TextField();

	void redraw(sf::RenderWindow *);

    bool is_intersected(int, int);
};

class InputField : public TextField {
private:

	std::size_t max_string_length;
public:

	bool isActive = false;

    InputField();

	InputField(int, int, int, int, int, int, sf::Font *, unsigned int, sf::Color *color);

	~InputField();

	void work(sf::Event event);

	sf::String take_text();
};

class UsersInterface {
public:
	std::vector<std::unique_ptr<InputField>> fields;
	int fields_count;
	std::vector<std::unique_ptr<TextField>> tfields;
	int tfields_count;
	std::vector<std::unique_ptr<Button>> buttons;
	int buttons_count;
    std::unique_ptr<sf::Font> font;
	int width, height;


	template<typename T>
	void make_array(std::vector<std::unique_ptr<T>>, int);

	template<typename T>
	void redraw_array(std::vector<T*>, int, sf::RenderWindow *);

	UsersInterface(int, int, std::string);

    void setShape(int, int);

    void addButton(int, int, int, int, std::string, int(*)(), bool);

    void addField(int, int, int, int, int, sf::Color *, bool, sf::String);

    int work(sf::RenderWindow *, sf::Event);

    sf::String take_message();

    void redraw(sf::RenderWindow *);

	~UsersInterface();
};

class ShowMessage {
private:
    int pos[2];
	std::unique_ptr<Message> content;
	std::unique_ptr<TextField> text;
public:
    int get_y();

    ShowMessage(int, int, int, int, sf::Color *, sf::Font *, Message *);

    ~ShowMessage();

    void draw(sf::RenderWindow *, int);
};

int cut_message(sf::String &, size_t);

class MessageList {
private:
	std::unique_ptr<sf::Color> my_message_color;
	std::unique_ptr<sf::Color> others_message_color;
	std::unique_ptr<sf::Font> font;
	std::vector<std::unique_ptr<ShowMessage>> messages;
	int width;
	int height;
	int length;
	int position;
	int messages_count = 0;

public:

	MessageList(int, int, int, int, int, int, int, int, std::string);

	~MessageList();

	void append(Message *, Sender);

	int step(int);

	void redraw(sf::RenderWindow *);
};

class Window {
private:
	sf::Vector2u shape;
	sf::Vector2i pos;
	std::unique_ptr<sf::Texture> fillpicture;
	std::string title;
	std::string path_to_picture;
	std::unique_ptr<MessageList> messages;
	std::unique_ptr<sf::RenderWindow> form;
	int workint = 0;
public:

    std::queue<IEvent*> eventQueue;

	std::unique_ptr<UsersInterface> inter_launch;

    std::unique_ptr<UsersInterface> inter_wait;

    std::unique_ptr<UsersInterface> inter_now;

	Window(unsigned int, unsigned int, int, int, std::string, std::string, std::string, std::string);

	~Window();

    void init(unsigned int, unsigned int, int, int,  std::string);

	void redraw();

	void close(IEventSender *);

	int open(IEventSender *);

	void work(IEventSender *);

	void wait_work(IEventSender *);

	void launch_work(IEventSender *);
};

class UI  : public IEventListener {
private:
	IEventSender *event_sender;
	IEventSubscriber *event_subscriber;
public:

	std::unique_ptr<Window> main_window;

	void get_event(IEvent* new_event) override;

	UI();

    UI(unsigned int, unsigned int, int, int,  std::string, std::string, std::string, std::string);

	void init(unsigned int, unsigned int, int, int,  std::string);

    //static void run(UI *);
    void run();

    void setEventSender(EventManager*);

    void setEventSubscriber(EventManager*);

	~UI();

};
