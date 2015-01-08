#ifndef FLAGWINDOW_H
#define FLAGWINDOW_H

#include <QMainWindow>

namespace Ui {
class FlagWindow;
}

class FlagWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FlagWindow(QWidget *parent = 0);
    ~FlagWindow();

private:
    Ui::FlagWindow *ui;
};

#endif // FLAGWINDOW_H
