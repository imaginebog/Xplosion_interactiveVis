#include "gllegendwidget.h"


GLLegendWidget::GLLegendWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

GLLegendWidget::~GLLegendWidget()
{

}

void GLLegendWidget::initializeGL()//initGL
{
    psysteminitialized=false;
    refreshLegend();
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
      glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

}
void GLLegendWidget::paintGL() {

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
      glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
    //refreshLegend();
    paintColorBoxScale(currentVarName,coloresScale,valoresScale,lenCol);

    glFlush();


}
void GLLegendWidget::mousePressEvent(QMouseEvent *event)
{
    //TODO
}
void GLLegendWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //TODO
}
void GLLegendWidget::mouseMoveEvent(QMouseEvent *event)
{
    //TODO
}
void GLLegendWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {

    }

}
void GLLegendWidget::refreshLegend()
{
    if(psysteminitialized)
    {
        lenCol=psystem->totalValuesScale;
        coloresScale=psystem->getColorsScale();
        currentVarName=psystem->getCurrentVarName();
        valoresScale=psystem->getValuesScale();
    }
    else
    {
        lenCol=3;
        coloresScale=(float**)calloc(3,sizeof(float*));
        coloresScale[0]=new float[3];
        coloresScale[0][0]=0;
        coloresScale[0][1]=0;
        coloresScale[0][2]=1;
        coloresScale[1]=new float[3];
        coloresScale[1][0]=0;
        coloresScale[1][1]=1;
        coloresScale[1][2]=0;
        coloresScale[2]=new float[3];
        coloresScale[2][0]=1;
        coloresScale[2][1]=0;
        coloresScale[2][2]=0;

        currentVarName="Any var";

        valoresScale=new float[3];
        valoresScale[0]=0;
        valoresScale[1]=10;
        valoresScale[2]=30;

    }
    update();
    printf("legend refreshed:\n");    printf("%s\n",currentVarName);
    printf("valores:(%f,%f,%f,%f)",valoresScale[0],valoresScale[1],valoresScale[2]);
    fflush(stdout);
}
void GLLegendWidget::setSimulSystem(ParticleSystem *ssystem)
{
    psystem=ssystem;
    psysteminitialized=true;
    refreshLegend();
}

void GLLegendWidget::paintText(QString str,float xStr,float yStr)
{
    float xS=xStr-1,yS=yStr-1;
    xS*=-width()/2,yS*=-height()/2;
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Calibri", 9));
    QFontMetrics fm(painter.font());
    int pixelsWide = fm.width(str);
    if(yS<=0)
        yS+=12;
    if(xS+pixelsWide>width())
        xS=width()-pixelsWide;
    QPointF stP= QPointF(xS,yS);
    painter.drawText(stP,str);
}

void GLLegendWidget::paintColorBoxScale(const char *nameVar, float** colors,float* values,int length)
{
    float heightTotal=1.8f, diffTotal=values[length-1]-values[0];
    float width=2;
    float gradHeight = heightTotal/(length-1), gradWidth = 1.8f;

    float posY=-1;
    char * testing=(char*)calloc(20,sizeof(char));
    int var=0;
    for (var = 0; var < length-1; ++var) {
        gradHeight=(values[var+1]-values[var])*heightTotal/diffTotal;
        float * col1=colors[var];
        float * col2=colors[var+1];
        glBegin(GL_QUADS);
        glColor3fv(col1);
        glVertex2f(-1, posY);
        glVertex2f(0.6f , posY);
        glColor3fv(col2);
        glVertex2f(0.6f , posY+gradHeight);
        glVertex2f(-1, posY+gradHeight);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);

        testing=(char*)calloc(20,sizeof(char));
        snprintf(testing,20,"%.2f",values[var]);
        paintText(testing,-0.6f,posY);

        posY+=gradHeight;
    }
    testing=(char*)calloc(20,sizeof(char));
    snprintf(testing,20,"%.2f",values[var]);
    paintText(testing,-0.6f,posY);
    paintText(nameVar,0.9f,posY+0.1f);
}
