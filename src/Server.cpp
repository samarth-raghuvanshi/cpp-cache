#include "Server.h"
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

Server::Server(int port, CommandParser &parser): port(port), parser(parser) {}

void Server::start() {
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0) {
        std::cerr << "WSA Startup Failed.\n";
        return;
    }
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return;
    }
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr))==SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    if (listen(listenSocket, SOMAXCONN)==SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    std::cout << "Server is listening on port " << port << "\n";
    while(true) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed.\n";
            closesocket(listenSocket);
            WSACleanup();
            return;
        }
        std::cout << "Client connected.\n";
        handleClient(clientSocket);
        std::cout << "Client disconnected.\n";
    }
    closesocket(listenSocket);
    WSACleanup();
    return;
}

void Server::handleClient(SOCKET clientSocket) {
    constexpr int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    std::string command;
    while(true) {
        while (true) {
            int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Receive failed.\n";
                closesocket(clientSocket);
                return;
            }
            if(bytesReceived<=0) {
                std::cerr << "Client Disconnected.\n";
                closesocket(clientSocket);
                return;
            }
            command.append(buffer, bytesReceived);
            if(!command.empty() && command.back()=='\n') {
                while(!command.empty() && (command.back()=='\n' || command.back()=='\r'))
                    command.pop_back();
                break;
            }
        }
        auto response = parser.process(command);
        response.response += "__END_RESPONSE__\n";
        send(clientSocket, response.response.c_str(), static_cast<int>(response.response.size()), 0);
        command.clear();
        if(response.toExit)
            break;
    }
    closesocket(clientSocket);
    return;
}