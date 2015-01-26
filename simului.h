#ifndef SIMULUI_H
#define SIMULUI_H

#include <QMainWindow>
#include "particleSystem.h"
#include "glwidget.h"


namespace Ui {
class SimulUI;
}

class SimulUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulUI(QWidget *parent = 0);
    ~SimulUI();
    void setSimulSystem(ParticleSystem *ssystem);
    void setSimulWindow(GLWidget *wid);
    void setDefaults();

private slots:
    void on_rbView_toggled(bool checked);

    void on_rbSpheres_toggled(bool checked);

    void on_rbFlatSpheres_toggled(bool checked);

    void on_rbPoints_toggled(bool checked);

    void on_rbVectors_toggled(bool checked);

    void on_butPlayStopAxis_clicked();

    void on_gbCutAxis_toggled(bool arg1);

    void on_rbXAxis_toggled(bool checked);

    void on_rbYAxis_toggled(bool checked);

    void on_rbZAxis_toggled(bool checked);

    void on_cbClipBox_toggled(bool checked);

    void on_butHistogram_clicked();

    void on_gbObj_toggled(bool arg1);

    void on_butPlayPauseTime_clicked();

    void on_sliderOpacitySimul_sliderReleased();

    void on_sliderOpacityObj_sliderReleased();

    void on_cbCurrentVar_currentIndexChanged(int index);

    void on_butResetView_clicked();

private:
    Ui::SimulUI *ui;
    GLWidget *simulWid;

    int axisToCut;
    bool playAxisCut;
};

#endif // SIMULUI_H
