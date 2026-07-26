#pragma once
#include <string>
#include <sstream>
#include <string_view>
#include "Cache.h"

struct CommandResponse {
    std::string response;
    bool toExit = false;
};

class CommandParser {
    public:
        static inline constexpr std::string_view HELP_MESSAGE = 
        "\n\t                          Available Commands\n\n"
        "\tSET <key> <value> [TTL <seconds>]        Store a key-value pair [with optional time-to-live]\n"
        "\tGET <key>                                Retrieve the value at key\n"
        "\tEXISTS <key>                             Check if key exists in cache\n"
        "\tDELETE <key>                             Delete the value at key if it exists\n"
        "\tRESIZE <integer_value>                   Resize cache capacity\n"
        "\tEXPIRE <key> <seconds>                   Set time-to-live for existing key\n"
        "\tPERSIST <key>                            Remove TTL expiration for existing key\n"
        "\tTTL <key>                                Retrieve time until expiration for existing key\n"
        "\tSAVE <filename>                          Save cache entries to ./data/filename(.db)\n"
        "\tLOAD <filename>                          Load cache entries from ./data/filename(.db)\n"
        "\tCLEAR                                    Clear the cache\n"
        "\tINFO                                     Print current capacity and number of entries\n"
        "\tHELP                                     Display list of commands\n"
        "\tEXIT                                     Exit the program\n";

        explicit CommandParser(Cache &cache);
        CommandResponse process(const std::string &line);

    private:
        Cache &cache;
        CommandResponse handleSet(std::stringstream &ss);
        CommandResponse handleGet(std::stringstream &ss);
        CommandResponse handleExists(std::stringstream &ss);
        CommandResponse handleDelete(std::stringstream &ss);
        CommandResponse handleResize(std::stringstream &ss);
        CommandResponse handleExpire(std::stringstream &ss);
        CommandResponse handlePersist(std::stringstream &ss);
        CommandResponse handleTTL(std::stringstream &ss);
        CommandResponse handleSave(std::stringstream &ss);
        CommandResponse handleLoad(std::stringstream &ss);
        CommandResponse handleClear(std::stringstream &ss);
};