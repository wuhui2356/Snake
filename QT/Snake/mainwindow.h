#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "thread.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

typedef struct{
    int color;
}MAP;

typedef struct SNAKE{
    bool isHead;
    int color;
    int x, y;
    SNAKE *next;
    SNAKE *previous;
}SNAKE;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void paintEvent(QPaintEvent *event);
    void insertSnakeBody(int x, int y, int color = SNAKEBODY);
    void gameInit();
    void changeMap(int x, int y, int color);
    void keyPressEvent(QKeyEvent *event);
    void createFood();
    void destroySnake();

private slots:
    void moveSlot();
    void recvKeyData(QString recvData);


private:
    Ui::MainWindow *ui;
    enum { MaxH = 40, MaxV = 30};
    enum { BackGround = 0, Wall = 110, SNAKEHEAD = 120, SNAKEBODY = 130, FOOD = 140, };
    enum { UP = 510, DOWN = 520, LEFT = 530, RIGHT = 540, };
    enum { RUP = 1, RDOWN = 2, RLEFT = 3, RRIGHT = 4, RA = 5, RB = 6, RC = 7, RD = 8, RR = 9, RS = 10, };
    MAP map[MaxV][MaxH];

    int direction;
    unsigned int score;

    const int Width = 20;
    const int Height = 20;

    int timerInterval;

    QMessageBox *message;

    QTimer *timer;
    Thread *thread;

    SNAKE *head;
    SNAKE *tail;
};

#endif // MAINWINDOW_H
