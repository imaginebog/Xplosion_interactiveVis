#ifndef MYGLUTTHREAD_H
#define MYGLUTTHREAD_H

#include <qthread.h>
#include <GL/freeglut.h>


class MyGLutThread : public QThread
{
    Q_OBJECT

protected:
    void run();
};

#endif // MYGLUTTHREAD_H
