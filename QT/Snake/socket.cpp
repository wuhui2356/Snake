#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


Socket::Socket(int port, QString address)
{
    this->PORT = port;
    this->ADDRESS = address;
}

Socket::~Socket()
{
    close(sockfd);
}

int Socket::InitSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("socket() err");
        return -1;
    }

    int on = 1;
    if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        perror("setsockopt() err");
        return -1;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr(ADDRESS.toStdString().c_str());
    if(-1 == bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)))
    {
        perror("bind() err");
        return -1;
    }

    if(-1 == listen(sockfd, MAXNUM))
    {
        perror("listen() err");
        return -1;
    }

    return 0;
}
