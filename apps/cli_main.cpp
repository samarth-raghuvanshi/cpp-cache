#include <iostream>
#include <string>
#include "Cache.h"
#include "CommandParser.h"

int main() {
    Cache cache(1000);
    CommandParser cmdParse(cache);
    std::string line;
    std::cout << "C++ MEMORY CACHE\n";
    std::cout << "Type HELP to view command usage.\n" << std::endl;
    while (true) {
        std::cout << "> ";
        if (!(std::getline(std::cin, line)))
            break;
        auto res = cmdParse.process(line);
        if (res.toExit)
            break;
        std::cout << res.response;
    }
    return 0;
}