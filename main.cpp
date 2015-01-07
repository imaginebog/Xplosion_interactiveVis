#include "mainwindow.h"
#include <QApplication>

int main3(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //printf("Good!");
    w.show();

    //MainWindow w2;
    //w2.show();

    return a.exec();
}
