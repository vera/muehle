#include "Board.cpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Board b;

    b.show();

    return a.exec();
}
