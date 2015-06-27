#include <QApplication>
#include <QtWidgets>
#include <QtSql>
#include "Mainwindow.h"
#include <QDebug>
#define W 1000
#define H 650

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /* Set window default position */
    QDesktopWidget *desktop = QApplication::desktop();
    int x = (desktop->width()-W)/2;
    int y = (desktop->height()-H)/4;
    Mainwindow w;
    w.move(x, y);
    w.show();
    w.askUserName();

    return a.exec();
}
