#include <iostream>
#include <string>
#include "Cache.h"
#include "CommandParser.h"
#include "Color.h"

int main() {
    Cache cache(1000);
    CommandParser cmdParse(cache);
    std::string line;
    std::cout << color::CYAN << "C++ MEMORY CACHE\n";
    std::cout << "Type HELP to view command usage.\n" << color::RESET << std::endl;
    while (true) {
        std::cout << color::CYAN << "> ";
        if (!(std::getline(std::cin, line)))
            break;
        if (!cmdParse.process(line))
            break;
    }
    std::cout << color::RESET;
    return 0;
}


