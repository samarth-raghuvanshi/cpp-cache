#include "Client.h"
#include <string>
#include <iostream>

int main() {
    const std::string localhost = "127.0.0.1";
    std::string host;
    int port;
    std::cout << "C++ MEMORY CACHE\n\n";
    std::cout << "Enter server address: ";
    std::getline(std::cin, host);
    std::cout << "Enter port number: ";
    std::cin >> port;
    std::cin.get();
    if (host.empty())
        host = localhost;
    Client client(host, port);
    client.start();
    return 0;
}