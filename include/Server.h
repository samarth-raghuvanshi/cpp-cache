#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "CommandParser.h"

class Server {
    public:
        Server(int port, CommandParser &parser);
        void start();
    private:
        void handleClient(SOCKET clientSocket);
        int port;
        CommandParser &parser;
};