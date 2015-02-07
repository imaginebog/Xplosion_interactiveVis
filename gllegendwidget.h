#ifndef GLLEGENDWIDGET_H
#define GLLEGENDWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLContext>

#include <QMouseEvent>
#include <QKeyEvent>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include "paramgl.h"
#include "particleSystem.h"

class GLLegendWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLLegendWidget(QWidget *parent = 0);
    ~GLLegendWidget();
    void setSimulSystem(ParticleSystem *ssystem);
    void refreshLegend();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
    bool psysteminitialized;
    ParticleSystem *psystem;
    void paintColorBoxScale(const char *nameVar, float** colors,float* values,int length);
    void paintRule();


    void paintText(QString str,float xStr,float yStr);
    float* parseColor(char* col);

    int lenCol;
    float** coloresScale;
    float* valoresScale;
    const char* currentVarName;
    void insertColorValue(QMouseEvent *event);
    void toggleVisibility(QMouseEvent *event);
    void editColorValues();
};

#endif // GLLEGENDWIDGET_H
