#ifndef LOOP2_H
#define LOOP2_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class Loop2;
}

class Loop2 : public QDialog
{
    Q_OBJECT

public:
    explicit Loop2(QWidget *parent, Chrnob32API *api);
    ~Loop2();

    void refresh();

protected slots:
    void toggleLoop2Pump1();
    void toggleLoop2Pump2();
    void toggleLoop2Pump3();

    void loop2Inlet1Open();
    void loop2Inlet2Open();
    void loop2Inlet3Open();
    void loop2Inlet1Close();
    void loop2Inlet2Close();
    void loop2Inlet3Close();

    void loop2Outlet1Open();
    void loop2Outlet2Open();
    void loop2Outlet3Open();
    void loop2Outlet1Close();
    void loop2Outlet2Close();
    void loop2Outlet3Close();

private:
    Ui::Loop2 *ui;

    Chrnob32API *m_api;
};

#endif // LOOP2_H
