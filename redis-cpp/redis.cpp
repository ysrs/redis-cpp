#include "redis.h"
#include <iostream>
#include <string>
#include <sstream>


#pragma comment(lib, "ws2_32.lib")

Redis::Redis(const std::string& host, int port) : host(host), port(port), sockfd(INVALID_SOCKET)
{
    if (!InitializeWinsock())
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
    }
}

Redis::~Redis()
{
    Disconnect();
}

bool Redis::InitializeWinsock()
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

bool Redis::Connect()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket. Error code: " << WSAGetLastError() << std::endl;
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (InetPtonA(AF_INET, host.c_str(), &(serverAddr.sin_addr)) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported." << std::endl;
        closesocket(sockfd);
        return false;
    }

    if (connect(sockfd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Connection Failed. Error code: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        return false;
    }

    return true;
}

void Redis::Disconnect()
{
    if (sockfd != INVALID_SOCKET)
    {
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
    }
}

std::string Redis::ExecuteCommand(const std::string& command)
{
    if (sockfd == INVALID_SOCKET)
    {
        std::cerr << "Not connected to Redis server." << std::endl;
        return "";
    }

    std::string request = command + "\r\n";
    send(sockfd, request.c_str(), request.length(), 0);

    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    recv(sockfd, buffer, sizeof(buffer), 0);

    std::stringstream response(buffer);
    std::string result;
    response >> result;

    return result;
}