#include "mainwindow.h"
#include "Board.cpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    Board b;

    b.show();

    return a.exec();
}
