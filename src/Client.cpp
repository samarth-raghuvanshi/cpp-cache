#include "Client.h"
#include <iostream>

Client::Client(const std::string &host, int port) : host(host), port(port) {};

void Client::start() {
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0) {
        std::cerr << "WSA Startup Failed\n";
        return;
    }
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);
    if(connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection Failed.\n";
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
    std::cout << "\nTCP connection established to " << host << " at port " << port << "\n";
    std::cout << "Type HELP to view command usage.\n" << std::endl;
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!(std::getline(std::cin, line)))
            break;
        bool exiting = (line=="EXIT");
        line+='\n';
        int bytesSent = send(clientSocket, line.c_str(), static_cast<int>(line.size()), 0);
        if (bytesSent == SOCKET_ERROR || bytesSent == 0) {
            std::cerr << "Could not send data\n";
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
        std::string response;
        constexpr int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
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
            response.append(buffer, bytesReceived);
            if (response.find("__END_RESPONSE__") != std::string::npos)
                break;
        }
        const std::string marker = "__END_RESPONSE__\n";
        size_t pos = response.find(marker);
        if (pos != std::string::npos)
            response.erase(pos);
        std::cout << response;
        if (exiting)
            break;
    }
    closesocket(clientSocket);
    WSACleanup();
    return;
}