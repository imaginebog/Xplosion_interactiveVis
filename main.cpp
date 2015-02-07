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
    GLWidget w;
    w.setWindowTitle("Xplosion simulation");
    w.show();

    SimulUI wui;
    wui.setWindowTitle("Simulation dashboard");
    wui.setSimulSystem(w.getSimulationSystem());
    wui.setSimulWindow(&w);
    wui.show();
    wui.setDefaults();
    wui.setSimulSystem(w.getSimulationSystem());
    return a.exec();
}
