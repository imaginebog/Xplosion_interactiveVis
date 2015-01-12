#include "simului.h"
#include "ui_simului.h"

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
