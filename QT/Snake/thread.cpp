#include "thread.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

Thread::Thread()
{
    conn  = 0;
    keySocket = new Socket();
    keySocket->InitSocket();

    m_stopFlag = false;
}

Thread::~Thread()
{
    stop();
    delete keySocket;
}

void Thread::stop()
{
    QMutexLocker locker(&mutex);
    m_stopFlag = true;
}

void Thread::run()
{
    m_stopFlag = false;
    char read_buf[1024];
    int ret = 0;
    struct sockaddr clientAddr;
    socklen_t clientLen;

    sockfd = keySocket->getSocketfd();
    qDebug() << "thread run ... ";

    while(1){          // 接收消息，　然后调用signale
       conn = accept(sockfd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientLen);
       if(conn == -1)
       {
           continue;
       }

       qDebug() << "accept ...";
       while(1)
       {
           bzero(read_buf, 1024);
           ret = read(conn, read_buf, 1024);
           if(ret > 0)
           {
               qDebug() << "recv: " << read_buf;
               emit recvDateToKEY(QString(read_buf)); // 发送信号
           }
           else
           {
               close(conn);
               break;
           }
       }
    }
    m_stopFlag = false;
}
