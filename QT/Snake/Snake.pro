#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T16:02:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    thread.cpp \
    socket.cpp

HEADERS  += mainwindow.h \
    thread.h \
    socket.h

FORMS    += mainwindow.ui
