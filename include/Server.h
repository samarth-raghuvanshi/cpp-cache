#pragma once
#include "CommandParser.h"

class Server {
    public:
        Server(int port, CommandParser &parser);
        void start();
    private:
        int port;
        CommandParser &parser;
};