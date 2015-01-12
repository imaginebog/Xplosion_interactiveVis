#include <cmath>
#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{

}


void GLWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 102.0/255.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glewInit();

        if (!glewIsSupported(
                "GL_VERSION_2_0 GL_VERSION_1_5 GL_ARB_multitexture GL_ARB_vertex_buffer_object")) {
            fprintf(stderr, "Required OpenGL extensions missing.");
            exit(EXIT_FAILURE);
        }
        else{
            printf("glew ok!");
            fflush(stdout);
        }
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawStripes();
    drawStars();
    glFlush();
}

void GLWidget::drawStar(float fX, float fY)
{
    const float kfPi = 3.1415926535897932384626433832795;
    // draw ten triangles
    const float kfRadius = 0.0616/2.0;
    const float kfInnerRadius = kfRadius*(1.0/(sin((2.0*kfPi)/5.0)*2.0*cos(kfPi/10.0) + sin((3.0*kfPi)/10.0)));
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(fX, fY, 0.0);
    for (int iVertIndex = 0; iVertIndex < 10; ++iVertIndex)
    {
        float fAngleStart    = kfPi/2.0 + (iVertIndex*2.0*kfPi)/10.0;
        float fAngleEnd        = fAngleStart + kfPi/5.0;
        if (iVertIndex % 2 == 0)
        {
            glVertex3f(fX + kfRadius*cos(fAngleStart)/1.9, fY + kfRadius*sin(fAngleStart), 0.0);
            glVertex3f(fX + kfInnerRadius*cos(fAngleEnd)/1.9, fY + kfInnerRadius*sin(fAngleEnd), 0.0);
        } else
        {
            glVertex3f(fX + kfInnerRadius*cos(fAngleStart)/1.9, fY + kfInnerRadius*sin(fAngleStart), 0.0);
            glVertex3f(fX + kfRadius*cos(fAngleEnd)/1.9, fY + kfRadius*sin(fAngleEnd), 0.0);
        }
    }
    glEnd();
}

void GLWidget::drawStars()
{
    for (int iStarRow = 0; iStarRow < 9; ++iStarRow)
    {
        float fY = 6.0/13.0 + (iStarRow + 1)*((7.0/13.0)/10);
        // alternate between rows of five or six stars
        if (iStarRow % 2 == 0)
        {
            for (int iStarCol = 0; iStarCol < 6; ++iStarCol)
            {
                drawStar(iStarCol*((0.76/1.9)/6.0) + (0.76/1.9)/12.0, fY);
            }
        } else
        {
            for (int iStarCol = 0; iStarCol < 5; ++iStarCol)
            {
                drawStar((iStarCol + 1)*((0.76/1.9)/6.0), fY);
            }
        }
    }
}

void GLWidget::drawStripes()
{
    for (int iStripeIndex = 0; iStripeIndex < 13; ++iStripeIndex)
    {
        // Alternate stripe colors
        if (iStripeIndex % 2 == 0)
        {
            glColor3f(204.0/255.0, 0.0, 0.0);
        } else
        {
            glColor3f(1.0, 1.0, 1.0);
        }

        float fStartX    = 0.0;
        float fEndX    = 1.0;
        float fStartY    = iStripeIndex*(1.0/13.0);
        float fEndY    = (iStripeIndex + 1)*(1.0/13.0);

        // the last seven stripes are shorter
        if (iStripeIndex > 5)
        {
            fStartX = 0.76/1.9;
        }

        glBegin(GL_QUADS);
        glVertex3f(fStartX, fStartY, 0.0);
        glVertex3f(fEndX, fStartY, 0.0);
        glVertex3f(fEndX, fEndY, 0.0);
        glVertex3f(fStartX, fEndY, 0.0);
        glEnd();
    }
}
