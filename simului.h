#ifndef SIMULUI_H
#define SIMULUI_H

#include <QMainWindow>

namespace Ui {
class SimulUI;
}

class SimulUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulUI(QWidget *parent = 0);
    ~SimulUI();

private:
    Ui::SimulUI *ui;
};

#endif // SIMULUI_H
