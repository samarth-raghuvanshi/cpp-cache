#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

class Client {
    public:
        Client(const std::string &host, int port);
        void start();
    private:
        std::string host;
        int port;
};