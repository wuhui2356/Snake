#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <assert.h>
#include <QPainter>
#include <QRectF>
#include <QDebug>
#include <QMessageBox>
#include <iostream>
#include <QTime>

// 按键后需要判断方向是否正确，能否前行

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    timerInterval = 200;
    thread = new Thread;
    message = new QMessageBox;

    ui->setupUi(this);
    timer = new QTimer;
    gameInit();  // 游戏初始化
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(moveSlot()));
    QObject::connect(thread, SIGNAL(recvDateToKEY(QString)), this, SLOT(recvKeyData(QString)));
    thread->start();
}

void MainWindow::gameInit()
{
    // 初始化，先将地图初始化，然后再一次性画出整个地图
    memset(map, 0, sizeof(map));
    timerInterval = 200;
    timer->setInterval(timerInterval);
    // 此处还应 判断　head != null, 则销毁当前的蛇
    if(head != NULL)
    {
        destroySnake();
    }

    // 设置墙，最外圈, 横坐标或纵坐标为０或最大时，表示最外圈,每个方块都设置为chang,表示需要下次重画
    for(int i = 0; i < MaxV; ++i)
    {
        for(int j = 0; j < MaxH; ++j)
        {
            if(i == 0 || j == 0 || i == (MaxV - 1) || j == (MaxH - 1))     // 此处设为墙
            {
                map[i][j].color = Wall;
            }
        }
    }

    // 初始化蛇的位置和方向
    score = 0;
    direction = RIGHT;
    insertSnakeBody(MaxV / 2, 14, SNAKEHEAD);
    insertSnakeBody(MaxV / 2, 13);
    insertSnakeBody(MaxV / 2, 12);
    insertSnakeBody(MaxV / 2, 11);
    insertSnakeBody(MaxV / 2, 10);

    createFood(); // 初始化创建食物
    this->update();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete thread;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.translate(10, 10);

    for(int i = 0; i < MaxV; ++i)
    {
        for(int j = 0; j < MaxH; ++j)
        {
            switch(map[i][j].color)
            {
            case Wall:
                painter.setBrush(QColor(84, 60, 0, 150));
                    break;

            case BackGround:
                painter.setBrush(QColor(84, 60, 85, 150));
                break;

            case SNAKEHEAD:
                painter.setBrush(QColor(0, 60, 85, 150));
                break;

            case SNAKEBODY:
                painter.setBrush(QColor(84, 0, 85, 150));
                break;

            case FOOD:
                painter.setBrush(QColor(84, 60, 85, 50));
                break;

            default:
                    painter.setBrush(QColor(84, 60, 85, 150));
            }
            painter.drawRect(j * Height, i * Width, Width, Height);    // 绘制地图中的一个块
        }
    }
}


// 该改变地图中的数据
void MainWindow::changeMap(int x, int y, int color)
{
    map[x][y].color = color;
}

// 蛇的移动, 通过定时器移动,该函数为定时器的槽
// 将tail变成head
// 先判断能否向当前方向移动
// 修改蛇头标志，同时同步地图
// 判断是否撞墙以及咬到自身
void MainWindow::moveSlot()
{
    qDebug() << direction;
    qDebug() << "x: " << head->x << " " << head->next->x;
    qDebug() << "y: " << head->y << " " << head->next->y;
    int tx = tail->x;
    int ty = tail->y;
    switch(direction)
    {
    case RIGHT:
        tail->x = head->x;
        tail->y = head->y + 1;
        break;

    case LEFT:
        tail->x = head->x;
        tail->y = head->y - 1;
        break;

    case UP:
        tail->x = head->x - 1;
        tail->y = head->y;
        break;

    case DOWN:
        tail->x = head->x + 1;
        tail->y = head->y;
        break;

    default:
        return;
    }
    head->isHead = false;
    changeMap(head->x, head->y, SNAKEBODY);     // 同步地图，将原来的头画成Body

    head = head->previous;
    head->isHead = true;
    tail = tail->previous;

    int tColor = map[head->x][head->y].color;

    changeMap(head->x, head->y, SNAKEHEAD);     // 同步地图，新头
    changeMap(tx, ty, BackGround);          // 移动后，清除地图上原来的尾巴


    // 判断是否GAMEOVER, 如果游戏结束，停止计时器, 撞墙或者咬到自己
    if(tColor == Wall || tColor == SNAKEBODY)
    {
        // message->information(this, QString::fromUtf8("游戏"), QString::fromUtf8("游戏结束"));
        message->setIcon(QMessageBox::Information);
        message->setWindowTitle(QString::fromUtf8("游戏"));
        message->setText(QString::fromUtf8("游戏结束"));
        message->exec();
        timer->stop();
        return ;
    }

    // 判断是否吃到东西, 吃到东西就插入一个节点,再创建一个食物
    if(tColor == FOOD)
    {
        insertSnakeBody(tx, ty);
        createFood();
        if(timerInterval > 2)
        {
            timer->setInterval(timerInterval -= 5);
        }
        else
        {
            timer->setInterval(1);
        }
    }

    this->update();

}

void MainWindow::recvKeyData(QString recvData)
{
    qDebug() << "ui recv: " <<  recvData;
    bool ok = true;
    int tmp = recvData.toInt(&ok);
    if(!ok)
    {
        return;
    }
    switch(tmp)
    {
       case RUP:
            if(head->x != head->next->x + 1)
            {
                direction = UP;
            }
            break;
       case RDOWN:
            if(head->x != head->next->x - 1)
            {
                direction = DOWN;
            }
            break;
       case RLEFT:
            if(head->y != head->next->y + 1)
            {
                direction = LEFT;
            }
           break;
       case RRIGHT:
            if(head->y != head->next->y - 1)
            {
                direction = RIGHT;
            }
           break;
       case RA:
            message->accept();

           break;
       case RB:

           break;
       case RC:

           break;
       case RD:

           break;
       case RR:
            if(timer->isActive())
            {
               timer->stop();
               timer->setInterval(timerInterval);
            }
           gameInit();
           break;
       case RS:
            if(!timer->isActive())
            {
                timer->start();
            }
           break;
       default:

           break;
    }
}

// 第一次插入蛇头，以后都插入蛇尾
void MainWindow::insertSnakeBody(int x, int y, int color)
{
    SNAKE *p = new SNAKE;           //　创建一个节点，并初始化为蛇头
    p->x = x;
    p->y = y;
    p->color = color;

    if(head == NULL)        //  第一次插入的肯定为蛇头
    {
        tail = head = p;
        p->next = p;
        p->previous = p;
        p->isHead = true;
    }
    else
    {
        head->previous = p;
        p->previous = tail;

        tail->next = p;
        p->next = head;

        tail = p;
    }
    changeMap(p->x, p->y, p->color);    // 设置节点的颜色
}

// 判断是否能构向按键方向移动
void MainWindow::keyPressEvent(QKeyEvent *event)
{
//    qDebug() << "keyPress: ";
//    qDebug() << "x: " << head->x << " " << head->next->x;
//    qDebug() << "y: " << head->y << " " << head->next->y;
    if(event->key() == Qt::Key_Up)
    {
        if(head->x != head->next->x + 1)
        {
            direction = UP;
        }
    }
    else if(event->key() == Qt::Key_Down)
    {
        if(head->x != head->next->x - 1)
        {
            direction = DOWN;
        }
    }
    else if(event->key() == Qt::Key_Left)
    {
        if(head->y != head->next->y + 1)
        {
            direction = LEFT;
        }
    }
    else if(event->key() == Qt::Key_Right)
    {
        if(head->y != head->next->y - 1)
        {
            direction = RIGHT;
        }
    }
    else if(event->key() == Qt::Key_S)
    {
        if(!timer->isActive())
        {
            timer->start();
        }
    }
    else if(event->key() == Qt::Key_R)
    {
        if(timer->isActive())
        {
           timer->stop();
        }
       gameInit();
    }
    else
    {
        return;
    }
}

// 随机生成食物
void MainWindow::createFood()
{
    int x = 0, y = 0;
    do{
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));        // 随机种子
        x = qrand() % MaxV;                    // 随机数
        y = qrand() % MaxH;
    }while(map[x][y].color != BackGround);

    map[x][y].color = FOOD;
}

// 销毁蛇
void MainWindow::destroySnake()
{
    SNAKE *p = head->next;
    SNAKE *p2 = head->next;

    while(p != head)
    {
        p2 = p->next;
        delete p;
        p = p2;
    }
    delete p;
    head = NULL;
    tail = NULL;
}








