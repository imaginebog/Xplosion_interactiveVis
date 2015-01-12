#include "simului.h"
#include <QApplication>
#include "glwidget.h"
#include <QtOpenGL/QGLFormat>
#include <QOpenGLContext>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    glutInit(&argc, argv);
    GLWidget w;
    w.setWindowTitle("Xplosion simulation");
    w.show();

    SimulUI wui;
    wui.setWindowTitle("Panel de control");
    wui.show();


    return a.exec();
}
