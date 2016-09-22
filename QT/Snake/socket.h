#ifndef SOCKET_H
#define SOCKET_H

#include <socket.h>
#include <QString>

class Socket
{
public:
    Socket(int port = 4444, QString address = "0.0.0.0");
    ~Socket();
    int InitSocket();
    int getSocketfd() { return sockfd; }

private:
    int MAXNUM = 1;
    int PORT;
    QString ADDRESS;
    int sockfd;
};

#endif // SOCKET_H
