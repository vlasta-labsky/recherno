#ifndef DEAREATOR_H
#define DEAREATOR_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class Deareator;
}

class Deareator : public QDialog
{
    Q_OBJECT

public:
    explicit Deareator(QWidget *parent, Chrnob32API *api);
    ~Deareator();

    void refresh();

protected slots:
    void levelSetpointInc();
    void levelSetpointDec();
    void flowValveInc();
    void flowValveDec();
    void drainValveInc();
    void drainValveDec();

    void levelManual();
    void levelAuto();

private:
    Ui::Deareator *ui;

    Chrnob32API *m_api;
};

#endif // DEAREATOR_H
