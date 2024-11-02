#include "turbine.h"
#include "ui_turbine.h"

#include "api.h"

#include <QDebug>

Turbine::Turbine(QWidget *parent, Chrnob32API *api) :
    QDialog(parent),
    ui(new Ui::Turbine),
    m_api(api)
{
    ui->setupUi(this);

    connect(ui->buttonSpeedStop, &QPushButton::clicked, this, &Turbine::speedStop);
    connect(ui->buttonSpeed900,  &QPushButton::clicked, this, &Turbine::speed900);
    connect(ui->buttonSpeed1800, &QPushButton::clicked, this, &Turbine::speed1800);
    connect(ui->buttonSpeed2700, &QPushButton::clicked, this, &Turbine::speed2700);
    connect(ui->buttonSpeed3600, &QPushButton::clicked, this, &Turbine::speed3600);

    connect(ui->buttonSlow, &QPushButton::clicked, this, &Turbine::runupSlow);
    connect(ui->buttonMedium, &QPushButton::clicked, this, &Turbine::runupMedium);
    connect(ui->buttonFast, &QPushButton::clicked, this, &Turbine::runupFast);

    connect(ui->buttonValveAuto, &QPushButton::clicked, this, &Turbine::valveAuto);
    connect(ui->buttonValveManual, &QPushButton::clicked, this, &Turbine::valveManual);

    connect(ui->buttonValveOpen, &QPushButton::clicked, this, &Turbine::valveOpen);
    connect(ui->buttonValveClose, &QPushButton::clicked, this, &Turbine::valveClose);

    connect(ui->buttonValveStartup, &QPushButton::clicked, this, &Turbine::valveStartup);
    connect(ui->buttonValveMain, &QPushButton::clicked, this, &Turbine::valveMain);

    connect(ui->buttonTrip, &QPushButton::clicked, this, &Turbine::trip);
    connect(ui->buttonReset, &QPushButton::clicked, this, &Turbine::reset);

    connect(ui->buttonTurningOff, &QPushButton::clicked, this, &Turbine::turningOff);
    connect(ui->buttonTurningOn, &QPushButton::clicked, this, &Turbine::turningOn);

    connect(ui->buttonBreakerClose, &QPushButton::clicked, this, &Turbine::breakerClose);
    connect(ui->buttonBreakerOpen, &QPushButton::clicked, this, &Turbine::breakerOpen);
}

Turbine::~Turbine()
{
    delete ui;
}

void Turbine::refresh()
{
    float rpm = std::get<0>(m_api->GETTRB(9));
    float power = std::get<0>(m_api->GETTRB(15));
    float expansion = std::get<0>(m_api->GETTRB(16));

    ui->lineSpeed->setText(QString::number(rpm, 'f', 0));
    ui->lineLoad->setText(QString::number(power, 'f', 1));
    ui->lineExpansion->setText(QString::number(expansion, 'f', 3));

    int speedSetpoint = std::get<0>(m_api->GETTRB(4));
    ui->lineSpeedSetpoint->setText(QString::number(speedSetpoint, 'f', 0));

    ui->buttonSpeedStop->setChecked(speedSetpoint == 0);
    ui->buttonSpeed900->setChecked(speedSetpoint == 900);
    ui->buttonSpeed1800->setChecked(speedSetpoint == 1800);
    ui->buttonSpeed2700->setChecked(speedSetpoint == 2700);
    ui->buttonSpeed3600->setChecked(speedSetpoint == 3600);

    float pressureSetpoint = std::get<0>(m_api->GETTRB(3)) - 101.3;
    ui->linePressureSetpoint->setText(QString::number(pressureSetpoint, 'f', 1));

    float drumPressure = m_api->GETDRM(0) - 101.3;
    ui->linePressure->setText(QString::number(drumPressure, 'f', 0));

    int valveAuto = std::get<1>(m_api->GETTRB(0));
    ui->buttonValveAuto->setChecked(valveAuto == -1);
    ui->buttonValveManual->setChecked(valveAuto == 0);

    float startupValveSignal = std::get<0>(m_api->GETTRB(1));
    float mainValveSignal = std::get<0>(m_api->GETTRB(2));
    int currentValve = std::get<1>(m_api->GETTRB(1));

    ui->buttonValveStartup->setChecked(currentValve == 1);
    ui->buttonValveMain->setChecked(currentValve == 2);

    float valveSignal = currentValve == 1 ? startupValveSignal : mainValveSignal;
    ui->lineValveSignal->setText(QString::number(valveSignal*100, 'f', 1)+"%");

    ui->lineValveStartup->setText(QString::number(startupValveSignal*100, 'f', 1)+"%");
    ui->lineValveMain->setText(QString::number(mainValveSignal*100, 'f', 1)+"%");

    int runupSpeed = std::get<1>(m_api->GETTRB(21));
    ui->buttonSlow->setChecked(runupSpeed == 1);
    ui->buttonMedium->setChecked(runupSpeed == 2);
    ui->buttonFast->setChecked(runupSpeed == 3);

    int turbineTrip = std::get<1>(m_api->GETTRB(6));
    ui->buttonTrip->setChecked(turbineTrip == -1);
    ui->buttonReset->setChecked(turbineTrip == 0);

    int breakerClosed = std::get<1>(m_api->GETTRB(5));
    ui->buttonBreakerClose->setChecked(breakerClosed == -1);
    ui->buttonBreakerOpen->setChecked(breakerClosed == 0);
}

void Turbine::speedStop()
{
    m_api->SETTRB(0, 4);
}

void Turbine::speed900()
{
    m_api->SETTRB(900, 4);
}

void Turbine::speed1800()
{
    m_api->SETTRB(1800, 4);
}

void Turbine::speed2700()
{
    m_api->SETTRB(2700, 4);
}

void Turbine::speed3600()
{
    m_api->SETTRB(3600, 4);
}

void Turbine::runupSlow()
{
    m_api->SETTRB(1, 11);
}

void Turbine::runupMedium()
{
    m_api->SETTRB(2, 11);
}

void Turbine::runupFast()
{
    m_api->SETTRB(3, 11);
}

void Turbine::valveAuto()
{
    m_api->SETTRB(1, 1);
}

void Turbine::valveManual()
{
    m_api->SETTRB(0, 1);
}

void Turbine::valveOpen()
{
    m_api->SETTRB(0.005, 2);
}

void Turbine::valveClose()
{
    m_api->SETTRB(-0.005, 2);
}

void Turbine::valveStartup()
{
    m_api->SETTRB(0, 5);
}

void Turbine::valveMain()
{
    m_api->SETTRB(0, 6);
}

void Turbine::trip()
{
    m_api->SETTRB(1, 8);
}

void Turbine::reset()
{
    m_api->SETTRB(0, 8);
}

void Turbine::turningOn()
{
    m_api->SETTRB(1, 10);
}

void Turbine::turningOff()
{
    m_api->SETTRB(0, 10);
}

void Turbine::breakerOpen()
{
    m_api->SETTRB(0, 7);
}

void Turbine::breakerClose()
{
    m_api->SETTRB(1, 7);
}
