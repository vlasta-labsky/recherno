#ifndef REACTOR_H
#define REACTOR_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class Reactor;
}

class Reactor : public QDialog
{
    Q_OBJECT

public:
    explicit Reactor(QWidget *parent, Chrnob32API *api);
    ~Reactor();

    void refresh();

    void powerAuto();
    void powerManual();

    void thermalOn();
    void thermalOff();

    void setpointInc();
    void setpointIncMore();
    void setpointDec();
    void setpointDecMore();

    void scramAutoToggle();
    void scramTrip();
    void scramReset();

private:
    Ui::Reactor *ui;

    Chrnob32API *m_api;
};

#endif // REACTOR_H
