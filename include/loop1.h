#ifndef LOOP1_H
#define LOOP1_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class Loop1;
}

class Loop1 : public QDialog
{
    Q_OBJECT

public:
    explicit Loop1(QWidget *parent, Chrnob32API *api);
    ~Loop1();

    void refresh();

protected slots:
    void toggleLoop1Pump1();
    void toggleLoop1Pump2();
    void toggleLoop1Pump3();

    void loop1Inlet1Open();
    void loop1Inlet2Open();
    void loop1Inlet3Open();
    void loop1Inlet1Close();
    void loop1Inlet2Close();
    void loop1Inlet3Close();

    void loop1Outlet1Open();
    void loop1Outlet2Open();
    void loop1Outlet3Open();
    void loop1Outlet1Close();
    void loop1Outlet2Close();
    void loop1Outlet3Close();

private:
    Ui::Loop1 *ui;

    Chrnob32API *m_api;
};

#endif // LOOP1_H
