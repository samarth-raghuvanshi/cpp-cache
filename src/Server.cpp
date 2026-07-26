#include "Server.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

Server::Server(int port, CommandParser &parser): port(port), parser(parser) {}

void Server::start() {
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0) {
        std::cerr << "WSA Startup Failed\n";
        return;
    }
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr))==SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
}