#include "condenser.h"
#include "ui_condenser.h"

#include "api.h"

#include <QDebug>

Condenser::Condenser(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::Condenser)
{
    ui->setupUi(this);

    connect(ui->buttonCondenserAirOn, &QPushButton::clicked, this, &Condenser::condenserAirOn);
    connect(ui->buttonCondenserAirOff, &QPushButton::clicked, this, &Condenser::condenserAirOff);

    connect(ui->buttonVacuumBreakerOpen, &QPushButton::clicked, this, &Condenser::vacuumBreakerOpen);
    connect(ui->buttonVacuumBreakerClose, &QPushButton::clicked, this, &Condenser::vacuumBreakerClose);

    connect(ui->buttonManual, &QPushButton::clicked, this, &Condenser::hotwellManual);
    connect(ui->buttonAuto, &QPushButton::clicked, this, &Condenser::hotwellAuto);

    connect(ui->buttonSetpointInc, &QPushButton::clicked, this, &Condenser::hotwellSetpointInc);
    connect(ui->buttonSetpointDec, &QPushButton::clicked, this, &Condenser::hotwellSetpointDec);
    connect(ui->buttonMakeupInc, &QPushButton::clicked, this, &Condenser::makeupInc);
    connect(ui->buttonMakeupDec, &QPushButton::clicked, this, &Condenser::makeupDec);

    connect(ui->buttonPump1, &QPushButton::clicked, this, &Condenser::togglePump1);
    connect(ui->buttonPump2, &QPushButton::clicked, this, &Condenser::togglePump2);
    connect(ui->buttonPump3, &QPushButton::clicked, this, &Condenser::togglePump3);
}

Condenser::~Condenser()
{
    delete ui;
}

void Condenser::refresh()
{
    float hotwellLevel = m_api->GETCND(34);
    ui->lineHotwellLevel->setText(QString::number(hotwellLevel, 'f', 1));

    float condenserAir = m_api->GETCND(21);
    ui->buttonCondenserAirOn->setChecked(condenserAir == 1.0);
    ui->buttonCondenserAirOff->setChecked(condenserAir == 0.0);

    float vacuumBreaker = m_api->GETCND(22);
    ui->buttonVacuumBreakerOpen->setChecked(vacuumBreaker == 1.0);
    ui->buttonVacuumBreakerClose->setChecked(vacuumBreaker == 0.0);

    float temp = m_api->GETCND(1);
    ui->lineTemperature->setText(QString::number(temp, 'f', 1));

    float ejectorFlow = m_api->GETCND(29);
    ui->lineSteamFlow->setText(QString::number(ejectorFlow, 'f', 1));

    float condenserVacuum = 101.3 - m_api->GETCND(0);
    ui->linePressure->setText(QString::number(condenserVacuum, 'f', 1));

    float makeupValve = std::get<0>(m_api->GETDAR(12));
    ui->lineMakeupValve->setText(QString::number(makeupValve*100, 'f', 1)+"%");

    float hotwellSetpoint = std::get<0>(m_api->GETDAR(30));
    ui->lineLevelSetpoint->setText(QString::number(hotwellSetpoint, 'f', 1));

    bool pump1 = m_api->GETCND(12);
    ui->buttonPump1->setChecked(pump1);

    bool pump2 = m_api->GETCND(13);
    ui->buttonPump2->setChecked(pump2);

    bool pump3 = m_api->GETCND(14);
    ui->buttonPump3->setChecked(pump3);

    float condensateFlow = m_api->GETCND(28);
    ui->lineCondensateFlow->setText(QString::number(condensateFlow, 'f', 0));

    bool autoManual = std::get<1>(m_api->GETDAR(6));
    ui->buttonAuto->setChecked(autoManual);
    ui->buttonManual->setChecked(!autoManual);
}

void Condenser::condenserAirOn()
{
    m_api->SETCND(1.0, 0);
}

void Condenser::condenserAirOff()
{
    m_api->SETCND(0.0, 0);
}

void Condenser::vacuumBreakerClose()
{
    m_api->SETCND(0.0, 1);
}

void Condenser::vacuumBreakerOpen()
{
    m_api->SETCND(1.0, 1);
}

void Condenser::hotwellAuto()
{
    uint16_t broken = m_api->GETMAL(59, 0);

    if(broken)
    {
        qDebug() << "hotwell auto broken, repairing";
        m_api->SETMAL(59, 0);
    }

    m_api->SETDAR(1.0, 16);
}

void Condenser::hotwellManual()
{
    m_api->SETDAR(0.0, 16);
}

void Condenser::hotwellSetpointInc()
{
    m_api->SETDAR(5.0, 13);
}

void Condenser::hotwellSetpointDec()
{
    m_api->SETDAR(-5.0, 13);
}

void Condenser::makeupInc()
{
    m_api->SETDAR(0.005, 7);
}

void Condenser::makeupDec()
{
    m_api->SETDAR(-0.005, 7);
}

void Condenser::togglePump1()
{
    bool broken = m_api->GETMAL(1, 0);
    if(broken)
    {
        qDebug() << "condensate pump 1 broken, repairing";
        m_api->SETMAL(1, 0);
    }

    bool pump1 = m_api->GETCND(12);
    m_api->SETPMP(11, !pump1);
}

void Condenser::togglePump2()
{
    bool broken = m_api->GETMAL(2, 0);
    if(broken)
    {
        qDebug() << "condensate pump 2 broken, repairing";
        m_api->SETMAL(2, 0);
    }

    bool pump2 = m_api->GETCND(13);
    m_api->SETPMP(12, !pump2);
}

void Condenser::togglePump3()
{
    bool broken = m_api->GETMAL(3, 0);
    if(broken)
    {
        qDebug() << "condensate pump 3 broken, repairing";
        m_api->SETMAL(3, 0);
    }

    bool pump3 = m_api->GETCND(14);
    m_api->SETPMP(13, !pump3);
}
