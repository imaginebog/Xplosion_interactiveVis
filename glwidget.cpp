#include <cmath>
#include "glwidget.h"



GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{

}
/*
 *
    glutDisplayFunc(display);//paint
    glutReshapeFunc(reshape);//resize
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(key);
    glutSpecialFunc(special);
    glutIdleFunc(idle);

    glutCloseFunc(cleanup);

    */

void GLWidget::initializeGL()//initGL
{
    glClearColor(0.0, 0.0, 102.0/255.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //TODO
    glFlush();
}
