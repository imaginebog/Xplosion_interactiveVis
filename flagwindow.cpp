#include "flagwindow.h"
#include "ui_flagwindow.h"

FlagWindow::FlagWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlagWindow)
{
    ui->setupUi(this);
}

FlagWindow::~FlagWindow()
{
    delete ui;
}
