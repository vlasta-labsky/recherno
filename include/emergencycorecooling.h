#ifndef EMERGENCYCORECOOLING_H
#define EMERGENCYCORECOOLING_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class EmergencyCoreCooling;
}

class EmergencyCoreCooling : public QDialog
{
    Q_OBJECT

public:
    explicit EmergencyCoreCooling(QWidget *parent, Chrnob32API *api);
    ~EmergencyCoreCooling();

    void refresh();

protected slots:
    void valveClose();
    void valveOpen();
    void valveAuto();

    void pump1Start();
    void pump1Stop();

    void pump2Start();
    void pump2Stop();

    void dieselStart();
    void dieselStop();

    void dieselInc();
    void dieselDec();

private:
    Ui::EmergencyCoreCooling *ui;

    Chrnob32API *m_api;
};

#endif // EMERGENCYCORECOOLING_H
