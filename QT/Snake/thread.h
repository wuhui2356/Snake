#ifndef THREAD
#define THREAD

#include "socket.h"
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

class Thread:public QThread
{
    Q_OBJECT
public:
    Thread();
    ~Thread();
    void stop();
    void run();

private:
    Socket *keySocket;
    int conn;
    int sockfd;
    bool m_stopFlag;
    QMutex mutex;



signals:
    void recvDateToKEY(QString str);
};

#endif // THREAD

