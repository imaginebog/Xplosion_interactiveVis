
#include "myglutthread.h"

void MyGLutThread::run()
{
    QApplication a(argc, argv);
    FlagWindow w;
    //printf("Good!");
    w.show();
    a.exec();
}

MyGLutThread::MyGLutThread(int argcp, char **argvp)//(QApplication aa)
{
    //aa.exec();
    argc=argcp;
    argv=argvp;
}
