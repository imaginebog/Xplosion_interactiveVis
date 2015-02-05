/****************************************************************************

****************************************************************************/



#include "colorvaluedialog.h"
ColorValueDialog::ColorValueDialog(float val, QColor color):ColorValueDialog()
{
    newValue=val;
    newColor=color;

    valueEdit->setText(QString::number(val));
    colorLabel->setText(color.name());
    colorLabel->setPalette(QPalette(color));
    colorLabel->setAutoFillBackground(true);

}

void ColorValueDialog::handleAddButton()
{
    valueDialog=valueEdit->text();
    colorDialog=colorLabel->text();
    accept();
}
ColorValueDialog::ColorValueDialog(QWidget *parent)
    : QDialog(parent)
{
    label = new QLabel(tr("Value:"));
    label2 = new QLabel(tr("Color:"));
    valueEdit = new QLineEdit;
    valueEdit->setValidator( new QDoubleValidator(this) );
    colorLabel = new QLabel;
    label->setBuddy(valueEdit);
    label2->setBuddy(colorLabel);


//! [1]
    addButton = new QPushButton(tr("&Insert"));
    addButton->setDefault(true);
    pickColorButton = new QPushButton(tr("Pick color"));
    pickColorButton->setDefault(true);
    connect(addButton, SIGNAL(clicked()),this, SLOT(handleAddButton()));
    connect(pickColorButton, SIGNAL(clicked()),this, SLOT(setColor()));



//! [4]
    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(valueEdit);

    QHBoxLayout *topLeftLayout2 = new QHBoxLayout;
    topLeftLayout2->addWidget(label2);
    topLeftLayout2->addWidget(pickColorButton);
    topLeftLayout2->addWidget(colorLabel);


    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(topLeftLayout2);
    leftLayout->addWidget(addButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->setRowStretch(2, 1);

    setLayout(mainLayout);
    setWindowTitle(tr("Insert Value-Color"));
}


void ColorValueDialog::setColor()
{
    const QColor color = QColorDialog::getColor(Qt::green, this, "Select Color",0);//, options);
    if (color.isValid()) {
        colorLabel->setText(color.name());
        colorLabel->setPalette(QPalette(color));
        colorLabel->setAutoFillBackground(true);
        newColor=color;
    }
}
//! [5]
