#include "simului.h"
#include "ui_simului.h"
#include <QDebug>
#include <QThread>
#include <QtWidgets>
SimulUI::SimulUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulUI)
{
    ui->setupUi(this);
}

SimulUI::~SimulUI()
{
    delete ui;
}

void SimulUI::setSimulSystem(ParticleSystem *ssystem)
{
    ui->scaleWidget->setSimulSystem(ssystem);
}
void SimulUI::setSimulWindow(GLWidget *wind)
{
    simulWid=wind;
}
void SimulUI::setDefaults()
{
    //Set cursor mode
    ui->rbView->setChecked(true);

    //Set visualization mode
    ui->rbFlatSpheres->setChecked(true);

    //set default axis to cut
    ui->rbXAxis->setChecked(true);
    axisToCut=0;
    playAxisCut=false;
}

void SimulUI::on_rbView_toggled(bool checked)
{
    qDebug() << (checked?"View mode!":"Clip mode!");

    if(checked)
    {
        QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_V,Qt::NoModifier);
        simulWid->keyPressEvent(qev);
    }
    else
    {
        QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_S,Qt::NoModifier);
        simulWid->keyPressEvent(qev);
    }
}

void SimulUI::on_rbSpheres_toggled(bool checked)
{
    if(checked)
    {
        QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_G,Qt::NoModifier);
        simulWid->keyPressEvent(qev);
    }

}

void SimulUI::on_rbFlatSpheres_toggled(bool checked)
{
    if(checked)
    {
        QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_B,Qt::NoModifier);
        simulWid->keyPressEvent(qev);
    }

}

void SimulUI::on_rbPoints_toggled(bool checked)
{
    if(checked)
    {
        QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_F,Qt::NoModifier);
        simulWid->keyPressEvent(qev);
    }

}

void SimulUI::on_rbVectors_toggled(bool checked)
{
    if(checked)
    {
        QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_N,Qt::NoModifier);
        simulWid->keyPressEvent(qev);
    }

}

void SimulUI::on_butPlayStopAxis_clicked()
{
    QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
    simulWid->keyPressEvent(qev);
}

void SimulUI::on_gbCutAxis_toggled(bool ch)
{
    if(ch)
    {

        if(ui->rbXAxis->isChecked())
        {
            QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_F1,Qt::NoModifier);
            simulWid->keyPressEvent(qev);
        }
        else if(ui->rbYAxis->isChecked())
        {
            QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_F2,Qt::NoModifier);
            simulWid->keyPressEvent(qev);
        }
        else
        {
            QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_F3,Qt::NoModifier);
            simulWid->keyPressEvent(qev);
        }
        QThread::msleep(300);
        ui->centralwidget->repaint();
    }
    else
    {
        QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_F4,Qt::NoModifier);
        simulWid->keyPressEvent(qev);
    }
}

void SimulUI::on_rbXAxis_toggled(bool checked)
{
    if(ui->gbCutAxis->isChecked()&&checked)
    {
        ui->centralwidget->repaint();
        QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_F1,Qt::NoModifier);
        simulWid->keyPressEvent(qev);
    }
}

void SimulUI::on_rbYAxis_toggled(bool checked)
{
    if(checked)
    {
        ui->centralwidget->repaint();
    QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_F2,Qt::NoModifier);
    simulWid->keyPressEvent(qev);


    }

}

void SimulUI::on_rbZAxis_toggled(bool checked)
{
    if(checked)
    {
        ui->centralwidget->repaint();
    QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_F3,Qt::NoModifier);
    simulWid->keyPressEvent(qev);

}
}

void SimulUI::on_cbClipBox_toggled(bool checked)
{

    QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_C,Qt::NoModifier);
    simulWid->keyPressEvent(qev);
}

void SimulUI::on_butHistogram_clicked()
{
    QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_H,Qt::NoModifier);
    simulWid->keyPressEvent(qev);
}

void SimulUI::on_gbObj_toggled(bool arg1)
{
    QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_O,Qt::NoModifier);
    simulWid->keyPressEvent(qev);
}

void SimulUI::on_butPlayPauseTime_clicked()
{
    QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_P,Qt::NoModifier);
    simulWid->keyPressEvent(qev);
}

void SimulUI::on_sliderOpacitySimul_sliderReleased()
{
    float valor=ui->sliderOpacitySimul->value();
    simulWid->setSimulOpacity(valor/100);
}

void SimulUI::on_sliderOpacityObj_sliderReleased()
{
    float valor=ui->sliderOpacityObj->value();
    simulWid->setObjOpacity(valor/100);
}

void SimulUI::on_cbCurrentVar_currentIndexChanged(int index)
{

    //QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_X,Qt::NoModifier);
    //simulWid->keyPressEvent(qev);
    printf("change var to %d\n",index);
    fflush(stdout);
    simulWid->changeCurrentVar(index);
    if(simulWid->vectorialMode())
    {
        ui->rbPoints->setEnabled(false);
        ui->rbSpheres->setEnabled(false);
        ui->rbFlatSpheres->setEnabled(false);
        ui->rbVectors->setEnabled(true);
        ui->rbVectors->setChecked(true);
    }
    else
    {
        ui->rbPoints->setEnabled(true);
        ui->rbSpheres->setEnabled(true);
        ui->rbFlatSpheres->setEnabled(true);
        ui->rbVectors->setEnabled(false);
        ui->rbFlatSpheres->setChecked(true);
    }
    ui->scaleWidget->refreshLegend();
}
void SimulUI::updateSimulView()
{
    simulWid->refreshView();
}

void SimulUI::on_butResetView_clicked()
{
    QKeyEvent *qev=new QKeyEvent(QEvent::KeyPress,Qt::Key_R,Qt::NoModifier);
    simulWid->keyPressEvent(qev);
}

void SimulUI::on_butStopTime_clicked()
{
    simulWid->setCurrentFrame(0);
}

void SimulUI::on_sliderTime_sliderReleased()
{
    //TODO released event is not enough, and time slider isn't updating with buttons or simul view interaction!
    float valor=ui->sliderTime->value();
    int nfram=valor*simulWid->getNumFrames()/100;
    simulWid->setCurrentFrame(nfram);
}

void SimulUI::on_pushButton_7_clicked()
{
    const QColor color = QColorDialog::getColor(Qt::green, this, "Select Color",0);//, options);
    if (color.isValid()) {
//        colorLabel->setText(color.name());
//        colorLabel->setPalette(QPalette(color));
//        colorLabel->setAutoFillBackground(true);
    }
}
