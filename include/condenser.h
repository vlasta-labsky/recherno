#ifndef CONDENSER_H
#define CONDENSER_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class Condenser;
}

class Condenser : public QDialog
{
    Q_OBJECT

public:
    explicit Condenser(QWidget *parent, Chrnob32API *api);
    ~Condenser();

    void refresh();

protected slots:
    void condenserAirOn();
    void condenserAirOff();

    void vacuumBreakerClose();
    void vacuumBreakerOpen();

    void hotwellAuto();
    void hotwellManual();

    void hotwellSetpointInc();
    void hotwellSetpointDec();

    void makeupInc();
    void makeupDec();

    void togglePump1();
    void togglePump2();
    void togglePump3();

private:
    Ui::Condenser *ui;

    Chrnob32API *m_api;
};

#endif // CONDENSER_H
