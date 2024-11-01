#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

class VirtualMachine;
class Chrnob32API;

class SystemSummary;
class FeedWater;
class Loop1;
class Loop2;
class Condenser;
class Deareator;
class Reactor;
class EmergencyCoreCooling;
class OfflineCooling;
class ReactorControlRods;
class TurbineSupport;
class Turbine;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void start();
    void stop();
    void restart();

    void systemSummary();
    void feedWater();
    void loop1();
    void loop2();
    void condenser();
    void deareator();
    void reactor();
    void emergencyCoreCooling();
    void offlineCooling();
    void controlRods();
    void turbineSupport();
    void turbine();

    void resetAlarms();

    void tick();

private:
    Ui::MainWindow *ui;

    VirtualMachine *m_machine;
    Chrnob32API *m_api;

    QTimer *m_timer;
    bool m_running;

    SystemSummary *m_systemSummary;
    FeedWater *m_feedWater;
    Loop1 *m_loop1;
    Loop2 *m_loop2;
    Condenser *m_condenser;
    Deareator *m_deareator;
    Reactor *m_reactor;
    EmergencyCoreCooling *m_ecc;
    OfflineCooling *m_offline;
    ReactorControlRods *m_rods;
    TurbineSupport *m_turbineSupport;
    Turbine *m_turbine;
};

#endif // MAINWINDOW_H
