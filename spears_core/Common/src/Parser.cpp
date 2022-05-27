#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

#include "Parser.h"

const std::string TEXT = "text";
const std::string TIME = "time";
const std::string CRYPTO_WORD = "cryptoWord";
const std::string LOGIN = "login";
const std::string TYPE = "type";
const std::string EMPTY_STR = "";


std::string clientMessage::parse() const
{
    boost::property_tree::ptree json;


    json.put(TEXT, this->text);
    json.put(TIME, this->time);
    json.put(CRYPTO_WORD, this->cryptoWord);

    if (json.empty())
        return EMPTY_STR;

    std::stringstream myJsonEncodedData;
    boost::property_tree::write_json(myJsonEncodedData, json);

    return myJsonEncodedData.str();
}

void parse_from_json(std::string jsonString, clientMessage& message)
{

    try {
        std::stringstream jsonEncodedData(jsonString);
        boost::property_tree::ptree json;

        boost::property_tree::read_json(jsonEncodedData, json);

        message.text = json.get<std::string>(TEXT, EMPTY_STR);
        message.time = json.get<std::string>(TIME, EMPTY_STR);
        message.cryptoWord = json.get<std::string>(CRYPTO_WORD, EMPTY_STR);
    }
    catch (std::exception const& error) {
        //std::cerr << error.what() << std::endl; 
    }
}


std::string serverMessage::parse() const
{
    boost::property_tree::ptree json;

    json.put(TEXT, this->text);
    json.put(TIME, this->time);
    json.put(LOGIN, this->login);
    json.put(TYPE, (size_t)(this->type));

    if (json.empty())
        return EMPTY_STR;

    std::stringstream myJsonEncodedData;
    boost::property_tree::write_json(myJsonEncodedData, json);

    return myJsonEncodedData.str();
}


void parse_from_json(std::string jsonString, serverMessage& message)
{

    try
    {
        std::stringstream jsonEncodedData(jsonString);
        boost::property_tree::ptree json;

        boost::property_tree::read_json(jsonEncodedData, json);

        message.text = json.get<std::string>(TEXT, EMPTY_STR);
        message.time = json.get<std::string>(TIME, EMPTY_STR);
        message.login = json.get<std::string>(LOGIN, EMPTY_STR);
        message.type = (Request)(json.get<size_t>(TYPE));

    }
    catch (std::exception const& error)
    {
        //std::cerr << error.what() << std::endl; 
    }
}