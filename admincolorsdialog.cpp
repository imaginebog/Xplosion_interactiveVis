/****************************************************************************

****************************************************************************/

#include "admincolorsdialog.h"
#include "colorvaluedialog.h"
AdminColorsDialog::AdminColorsDialog(ParticleSystem* psystemp):AdminColorsDialog()
{
    psystem=psystemp;
    label = new QLabel(tr("Select value-color"));
    colorvaluesCombobox=new QComboBox(this);
   updateComboBox();


   ParticleSystem::ColorValue currentcv=psystem->getColorValues()[colorvaluesCombobox->currentIndex()];
   newColor=new QColor(currentcv.colorRGB[0],currentcv.colorRGB[1]*255,currentcv.colorRGB[2]*255);
   newValue=currentcv.value;
//! [1]
    editButton = new QPushButton(tr("&New Color-Value"));
    editButton->setDefault(true);
    deleteButton = new QPushButton(tr("&Delete selected Color-Value"));
    deleteButton->setDefault(true);
    connect(editButton, SIGNAL(clicked()),this, SLOT(handleAddButton()));
    connect(deleteButton, SIGNAL(clicked()),this, SLOT(deleteColor()));



//! [4]
    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(colorvaluesCombobox);

    QHBoxLayout *topLeftLayout2 = new QHBoxLayout;
    topLeftLayout2->addWidget(deleteButton);
    //topLeftLayout2->addWidget(editButton);


    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(topLeftLayout2);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->setRowStretch(2, 1);

    setLayout(mainLayout);
    QString strtit(psystem->getCurrentVarName());
    setWindowTitle("List color-values in variable "+strtit);
}
void AdminColorsDialog::updateComboBox()
{
    ParticleSystem::ColorValue* colors=psystem->getColorValues();
    colorvaluesCombobox->clear();
    int countColors=psystem->getTotalColorValues();
    for (int i = 0; i < countColors; ++i) {
        QColor* tempo=new QColor(colors[i].colorRGB[0]*255,colors[i].colorRGB[1]*255,colors[i].colorRGB[2]*255);
        colorvaluesCombobox->addItem(QString::number(colors[i].value)+"  "+tempo->name());
    }
}


AdminColorsDialog::AdminColorsDialog(QWidget *parent)
    : QDialog(parent)
{

}

void AdminColorsDialog::handleAddButton()//change for an edit!
{
    //TODO
    accept();
}
void AdminColorsDialog::deleteColor()
{
    psystem->deleteColorValue(colorvaluesCombobox->currentIndex());
    updateComboBox();
}
//! [5]
