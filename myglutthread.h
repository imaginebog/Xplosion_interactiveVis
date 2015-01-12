#ifndef MYGLUTTHREAD_H
#define MYGLUTTHREAD_H

#include <qthread.h>
#include <GL/freeglut.h>
#include <QApplication>
#include "flagwindow.h"


class MyGLutThread : public QThread
{
    Q_OBJECT
public:
    MyGLutThread(int argcp, char **argvp);//(QApplication aa);
protected:
    void run();
private:
    int argc;
    char **argv;
};

#endif // MYGLUTTHREAD_H
