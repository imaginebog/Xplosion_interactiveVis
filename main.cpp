#include "simului.h"
#include <QApplication>
#include "glwidget.h"
#include "gllegendwidget.h"
#include <QtOpenGL/QGLFormat>
#include <QOpenGLContext>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    glutInit(&argc, argv);
    //GLWidget w;
    //w.setWindowTitle("Xplosion simulation");
    //w.show();

    GLLegendWidget w2;
    w2.setWindowTitle("The legend");
    w2.show();

    SimulUI wui;
    wui.setWindowTitle("Simulation dashboard");
    wui.show();


    return a.exec();
}
