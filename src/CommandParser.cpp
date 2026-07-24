#include "CommandParser.h"
#include "Color.h"
#include <iostream>

CommandParser::CommandParser(Cache &cache) :cache(cache) {}

bool CommandParser::process(const std::string &line) {
    std::stringstream ss(line);
    std::string command;
    if(!(ss >> command))
        return true;
    if (command == "SET")
        handleSet(ss);
    else if (command == "GET")
        handleGet(ss);
    else if (command == "EXISTS")
        handleExists(ss);
    else if (command == "DELETE")
        handleDelete(ss);
    else if (command == "RESIZE")
        handleResize(ss);
    else if (command == "HELP")
        std::cout << color::YELLOW << HELP_MESSAGE << color::RESET << std::endl;
    else if (command == "EXIT")
        return false;
    else 
        std::cout << color::RED << "\t\"" << command << "\" is not a recognised command.\n\tType HELP to view command usage." << color::RESET << std::endl;
    return true;
}

void CommandParser::handleResize(std::stringstream &ss) {
    long long newCapacity;
    std::string extra;
    if ((!(ss >> newCapacity)) || (ss >> extra) ) {
        std::cout << color::RED << "\tUsage: RESIZE <integer_value>" << color::RESET << std::endl;
        return;
    }
    if (newCapacity <= 0) {
        std::cout << color::RED << "\tINVALID: Capacity should be atleast 1" << color::RESET << std::endl;
        return;
    }
    int changed = cache.resize(static_cast<size_t>(newCapacity));
    std::cout << color::GREEN << "\tCapacity changed to " << newCapacity << ".\n\tEvicted " << changed << " entries." << color::RESET << std::endl;
}

void CommandParser::handleSet(std::stringstream &ss) {
    std:: string key, value, extra;
    if ((!(ss >> key >> value)) || (ss >> extra)) {
        std::cout << color::RED << "\tUsage: SET <key> <value>" << color::RESET << std::endl;
        return;
    }
    cache.set(key, value);
    std::cout << color::GREEN << "\tStored successfully." << color::RESET << std::endl;
}

void CommandParser::handleGet(std::stringstream &ss) {
    std::string key, extra;
    if ((!(ss >> key)) || (ss >> extra)) {
        std::cout << color::RED << "\tUsage: GET <key>" << color::RESET << std::endl;
        return;
    }
    auto s = cache.get(key);
    if (s.has_value())
        std::cout << color::GREEN << "\t" << s.value() << color::RESET<< std::endl;
    else
        std::cout << color::RED << "\tKEY: \""<< key << "\" not found" << color::RESET << std::endl;
}

void CommandParser::handleExists(std::stringstream &ss) {
    std::string key, extra;
    if ((!(ss >> key)) || (ss >> extra)) {
        std::cout << color::RED << "\tUsage: EXISTS <key>" << color::RESET << std::endl;
        return;
    }
    if (cache.exists(key))
        std::cout << color::GREEN << "\tKEY: \"" << key << "\" exists" << color::RESET << std::endl;
    else
        std::cout << color::GREEN << "\tKEY: \"" << key << "\" does not exist" << color::RESET << std::endl;
}

void CommandParser::handleDelete(std::stringstream &ss) {
    std::string key, extra;
    if ((!(ss >> key)) || (ss >> extra)) {
        std::cout << color::RED << "\tUsage: DELETE <key>" << color::RESET << std::endl;
        return;
    }
    cache.erase(key);
    std::cout << color::GREEN << "\tDeleted successfully." << color::RESET << std::endl;
}
