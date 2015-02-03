/****************************************************************************

****************************************************************************/

#include <QtWidgets>

#include "colorvaluedialog.h"
ColorValueDialog::ColorValueDialog(float val, float* color):ColorValueDialog()
{
    newValue=val;
    newColor=color;

    valueEdit->setText(QString::number(val));
    colorEdit->setText(QString::number(color[0])+","+QString::number(color[1])+","+QString::number(color[2]));

}

void ColorValueDialog::handleAddButton()
{
    valueDialog=valueEdit->text();
    colorDialog=colorEdit->text();
    accept();
}
ColorValueDialog::ColorValueDialog(QWidget *parent)
    : QDialog(parent)
{
    label = new QLabel(tr("Value"));
    label2 = new QLabel(tr("Color #RRGGBB"));
    valueEdit = new QLineEdit;
    valueEdit->setValidator( new QDoubleValidator(this) );
    colorEdit = new QLineEdit;
    label->setBuddy(valueEdit);
    label2->setBuddy(colorEdit);


//! [1]
    addButton = new QPushButton(tr("&Add"));
    addButton->setDefault(true);
    connect(addButton, SIGNAL(clicked()),this, SLOT(handleAddButton()));



//! [4]
    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(valueEdit);

    QHBoxLayout *topLeftLayout2 = new QHBoxLayout;
    topLeftLayout2->addWidget(label2);
    topLeftLayout2->addWidget(colorEdit);


    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(topLeftLayout2);
    leftLayout->addWidget(addButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->setRowStretch(2, 1);

    setLayout(mainLayout);

    setWindowTitle(tr("Add Value-Color"));
}
//! [5]
