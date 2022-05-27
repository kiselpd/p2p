#pragma once
#include <string>
#include "CommonTypes.h"

class Parseable
{
public:
    virtual std::string parse() const = 0;

    virtual ~Parseable() = default;
};


class clientMessage : public Parseable
{
public:
    std::string text;
    std::string time;
    std::string cryptoWord;

    std::string parse() const override;
};

void parse_from_json(std::string jsonString, clientMessage& message);

class serverMessage : public Parseable
{
public:
    std::string text;
    std::string time;
    std::string login;
    Request type;

    std::string parse() const override;
};

void parse_from_json(std::string jsonString, serverMessage& message);

template<typename T>
void get_request_body(std::string jsonString, T& body) {
    parse_from_json(jsonString, body);
}