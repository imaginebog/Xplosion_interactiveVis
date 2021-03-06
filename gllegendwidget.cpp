#include "simului.h"
#include "gllegendwidget.h"
#include "colorvaluedialog.h"
#include "admincolorsdialog.h"


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
    paintColorBoxScale(currentVarName,coloresScale,valoresScale,lenCol);

    glFlush();


}
void GLLegendWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        QMenu myMenu;

        QAction* insertAct=new QAction("Insert new Color - Value",this);
        myMenu.addAction(insertAct);
        QAction* editAct=new QAction("Edit Color-Values",this);
        myMenu.addAction(editAct);
        QAction* toggleAct=new QAction("Toggle visibility",this);
        myMenu.addAction(toggleAct);
        QAction* result=myMenu.exec(mapToGlobal(event->pos()));
        if(result)
        {
            if(result==insertAct)
                insertColorValue(event);
            else if(result==editAct)
                editColorValues();
            else if(result==toggleAct)
                toggleVisibility(event);
        }

        return;
    }
    else if(event->button()==Qt::LeftButton)
    {
        toggleVisibility(event);
    }
    refreshLegend();
    //((SimulUI)parentWidget()).updateSimulView();//TODO

}
void GLLegendWidget::toggleVisibility(QMouseEvent *event)
{
    float percent=(1-((float)event->y())/(float)height())/0.95f;
    float val=psystem->getValue(percent);
    int range=psystem->getRangeValue(val);
    psystem->toggleVisibility(range);
    refreshLegend();
}

void GLLegendWidget::editColorValues()
{
    AdminColorsDialog diag(psystem);
    diag.exec();
}

void GLLegendWidget::insertColorValue(QMouseEvent *event)
{
    float percent=(1-((float)event->y())/(float)height())/0.95f;
    float val=psystem->getValue(percent);
    char* colN=psystem->getColor(val);
    QString colorn=QString(colN);
    colorn.replace("0x","#");
    qDebug()<<colorn;
    ColorValueDialog diag(val,QColor(colorn));
    diag.exec();
    if(diag.result()==QDialog::Accepted)
    {
        QColor qcol=diag.newColor;
        float* insCol=(float*)calloc(3,sizeof(float));
        insCol[0]=(float)qcol.red()/255.0f;
        insCol[1]=(float)qcol.green()/255.0f;
        insCol[2]=(float)qcol.blue()/255.0f;
        psystem->insertColorValue(insCol,diag.newValue,psystem->currentVariable);
    }

}

void GLLegendWidget::refreshLegend()
{
    if(psysteminitialized)
    {
        lenCol=psystem->getTotalColorValues();
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
    float heightTotal=1.9f, diffTotal=values[length-1]-values[0];
    float gradHeight;// = heightTotal/(length);//-1),

    float posY=-1;
    char * testing=(char*)calloc(20,sizeof(char));
    int var=0;
    ParticleSystem::ColorValue* cvs=psystem->getColorValues();
    for (var = 0; var < length-1; ++var) {
        bool hid=cvs[var].hidden;
        gradHeight=(values[var+1]-values[var])*heightTotal/diffTotal;
        float * col1=colors[var];
        float * col2=colors[var+1];
        glBegin(GL_QUADS);
        glColor3fv(col1);
        glVertex2f(-1, posY);
        glVertex2f(0.4f , posY);
        glColor3fv(col2);
        glVertex2f(0.4f , posY+gradHeight);
        glVertex2f(-1, posY+gradHeight);
        glEnd();


        if(hid)
        {
            glColor3f(0,0,0);
            glBegin(GL_LINE_STRIP);
            glVertex2f(-1, posY+gradHeight);
            glVertex2f(-1, posY);
            glVertex2f(0.4f , posY);
            glVertex2f(0.4f , posY+gradHeight);
            glVertex2f(-1, posY+gradHeight);
            glVertex2f(0.4f , posY);
            glVertex2f(-1, posY);
            glVertex2f(0.4f , posY+gradHeight);

            glEnd();
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        testing=(char*)calloc(20,sizeof(char));
        snprintf(testing,20,"%.2f",values[var]);
        paintText(testing,-0.4f,posY);

        posY+=gradHeight;
    }
    testing=(char*)calloc(20,sizeof(char));
    snprintf(testing,20,"%.2f",values[var]);
    paintText(testing,-0.6f,posY);
    paintRule();
}
void GLLegendWidget::paintRule()
{
    //TODO paint according to real scale (lines are not corresponding to nothing).
    float heightTotal=1.9f;
    float gradHeight = heightTotal/20;

    float posY=-1;

    glColor3f(0,0,0);
    glBegin(GL_LINES);
    for (int var = 0; var < 20; ++var) {

        glVertex2f(0.2f, posY);
        glVertex2f(0.4f , posY);
        posY+=gradHeight;

    }
    glEnd();
}
