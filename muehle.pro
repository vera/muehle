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
        Player.cpp\
        HumanPlayer.cpp\
        AIPlayer.cpp\
        Board.cpp

HEADERS  +=\
        Player.h \
        Board.h \
        HumanPlayer.h \
        AIPlayer.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    README.md

RC_FILE = muehle.rc

TRANSLATIONS = muehle_de.ts \
             muehle_en.ts
