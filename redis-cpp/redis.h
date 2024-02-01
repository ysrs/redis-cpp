#ifndef REDIS_H
#define REDIS_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>


class Redis
{
public:
    Redis(const std::string& host, int port, const std::string& password, int db);
    ~Redis();

    bool Connect();
    void Disconnect();
    std::string ExecuteCommand(const std::string& command);

private:
    std::string host;
    int port;
    std::string password;
    int db;
    SOCKET sockfd;

    bool InitializeWinsock();
};

#endif // REDIS_H
