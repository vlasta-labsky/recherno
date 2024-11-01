#include "reactor.h"
#include "ui_reactor.h"

#include "api.h"

#include <math.h>

#include <QDebug>

Reactor::Reactor(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::Reactor)
{
    ui->setupUi(this);

    connect(ui->buttonAutoOff, &QPushButton::clicked, this, &Reactor::powerManual);
    connect(ui->buttonAutoOn, &QPushButton::clicked, this, &Reactor::powerAuto);

    connect(ui->buttonThermalOn, &QPushButton::clicked, this, &Reactor::thermalOn);
    connect(ui->buttonThermalOff, &QPushButton::clicked, this, &Reactor::thermalOff);

    connect(ui->buttonSetpointInc, &QPushButton::clicked, this, &Reactor::setpointInc);
    connect(ui->buttonSetpointIncMore, &QPushButton::clicked, this, &Reactor::setpointIncMore);

    connect(ui->buttonSetpointDec, &QPushButton::clicked, this, &Reactor::setpointDec);
    connect(ui->buttonSetpointDecMore, &QPushButton::clicked, this, &Reactor::setpointDecMore);

    connect(ui->buttonScramAuto, &QPushButton::clicked, this, &Reactor::scramAutoToggle);
    connect(ui->buttonScramTrip, &QPushButton::clicked, this, &Reactor::scramTrip);
    connect(ui->buttonScramReset, &QPushButton::clicked, this, &Reactor::scramReset);
}

Reactor::~Reactor()
{
    delete ui;
}

void Reactor::refresh()
{
    float powerSetpoint = m_api->GETSPT();
    ui->linePowerSetpoint->setText(QString::number(powerSetpoint*100, 'f', 1));

    bool thermalCorr = m_api->GTTCOR();
    ui->buttonThermalOff->setChecked(!thermalCorr);
    ui->buttonThermalOn->setChecked(thermalCorr);

    uint32_t manualAuto = m_api->GETAUT(1);
    ui->buttonAutoOn->setChecked(manualAuto);
    ui->buttonAutoOff->setChecked(!manualAuto);

    bool scram = m_api->GETAUT(2);
    ui->buttonScramTrip->setChecked(scram);
    ui->buttonScramReset->setChecked(!scram);

    bool autoScram = m_api->GETAUT(3);
    ui->buttonScramAuto->setChecked(autoScram);

    float neutronFlux = m_api->GETFLX();
    ui->lineFlux->setText(QString::number(neutronFlux*100, 'f', 1)+"%");

    float neutronFluxLog = log10(neutronFlux);
    ui->lineFluxLog->setText(QString::number(neutronFluxLog, 'f', 2));

    float fuelBurnup = m_api->GETBUN(-1, -1);
    ui->lineFuelBurnup->setText(QString::number(fuelBurnup*100, 'f', 1)+"%");

    float regErr = m_api->GETERR();
    ui->lineControlError->setText(QString::number(regErr*100, 'f', 1)+"%");

    float neutronRate = m_api->GETRAT(2);
    ui->lineNeutron->setText(QString::number(neutronRate*100, 'f', 1)+"%");

//    float wtf = m_api->GETMK();
//    qDebug() << "GETMK" << wtf;

    float thermal[25];
    m_api->GETTHR(thermal);

    float averageThermal = 0.0;
    for(int i = 0; i < 25; i++)
    {
        averageThermal += thermal[i];
    }
    averageThermal /= 25;

    ui->lineThermal->setText(QString::number(averageThermal*100, 'f', 1)+"%");
}

void Reactor::powerAuto()
{
    m_api->SETAUT(1);
}

void Reactor::powerManual()
{
    m_api->SETAUT(0);
}

void Reactor::thermalOn()
{
    m_api->STTCOR(1);
}

void Reactor::thermalOff()
{
    m_api->STTCOR(0);
}

void Reactor::setpointInc()
{
    m_api->SETSPT(0.005, 1);
}

void Reactor::setpointIncMore()
{
    m_api->SETSPT(0.05, 1);
}

void Reactor::setpointDec()
{
    m_api->SETSPT(-0.005, 1);
}

void Reactor::setpointDecMore()
{
    m_api->SETSPT(-0.05, 1);
}

void Reactor::scramAutoToggle()
{
    bool autoScram = m_api->GETAUT(3);
    m_api->TRIPIT(2, !autoScram);
}

void Reactor::scramTrip()
{
    m_api->TRIPIT(1, 1);
}

void Reactor::scramReset()
{
    m_api->TRIPIT(1, 0);
}
