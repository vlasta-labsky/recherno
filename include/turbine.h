#ifndef TURBINE_H
#define TURBINE_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class Turbine;
}

class Turbine : public QDialog
{
    Q_OBJECT

public:
    explicit Turbine(QWidget *parent, Chrnob32API *api);
    ~Turbine();

    void refresh();

protected slots:
    void speedStop();
    void speed900();
    void speed1800();
    void speed2700();
    void speed3600();

    void runupSlow();
    void runupMedium();
    void runupFast();

    void valveAuto();
    void valveManual();

    void valveOpen();
    void valveClose();

    void valveStartup();
    void valveMain();

    void trip();
    void reset();

    void turningOn();
    void turningOff();

    void breakerOpen();
    void breakerClose();

private:
    Ui::Turbine *ui;

    Chrnob32API *m_api;
};

#endif // TURBINE_H
