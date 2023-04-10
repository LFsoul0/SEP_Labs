#include "qsnake.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSnake s;
    s.show();
    return a.exec();
}
