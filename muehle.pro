#-------------------------------------------------
#
# Project created by QtCreator 2016-08-07T16:22:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = muehle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        Player.cpp\
        HumanPlayer.cpp\
        AIPlayer.cpp\
        Board.cpp

HEADERS  += mainwindow.h\
        Exceptions.h\
        Player.h \
    Board.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    README.md
