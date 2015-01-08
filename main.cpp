#include "flagwindow.h"
#include <QApplication>

int main3(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FlagWindow w;
    //printf("Good!");
    w.show();
    //w2.show();

    return a.exec();
}
