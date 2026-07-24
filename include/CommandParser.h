#pragma once
#include <string>
#include <sstream>
#include <string_view>
#include "Cache.h"

class CommandParser {
    public:
        static inline constexpr std::string_view HELP_MESSAGE = 
        "\n\tAvailable Commands:\n\n"
        "\tSET <key> <value>        Store a key-value pair\n"
        "\tGET <key>                Retrieve the value at key\n"
        "\tEXISTS <key>             Check if key exists in cache\n"
        "\tDELETE <key>             Delete the value at key if it exists\n"
        "\tRESIZE <integer_value>   Resize cache capacity\n"
        "\tINFO                     Print current capacity and number of entries\n"
        "\tHELP                     Display list of commands\n"
        "\tEXIT                     Exit the program\n";

        explicit CommandParser(Cache &cache);
        bool process(const std::string &line);

    private:
        Cache &cache;
        void handleSet(std::stringstream &ss);
        void handleGet(std::stringstream &ss);
        void handleExists(std::stringstream &ss);
        void handleDelete(std::stringstream &ss);
        void handleResize(std::stringstream &ss);
};