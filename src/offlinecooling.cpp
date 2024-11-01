#include "offlinecooling.h"
#include "ui_offlinecooling.h"

#include "api.h"

#include <QDebug>

OfflineCooling::OfflineCooling(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::OfflineCooling)
{
    ui->setupUi(this);

    connect(ui->buttonLoop1Inc, &QPushButton::clicked, this, &OfflineCooling::loop1Inc);
    connect(ui->buttonLoop1Dec, &QPushButton::clicked, this, &OfflineCooling::loop1Dec);
    connect(ui->buttonLoop2Inc, &QPushButton::clicked, this, &OfflineCooling::loop2Inc);
    connect(ui->buttonLoop2Dec, &QPushButton::clicked, this, &OfflineCooling::loop2Dec);

    connect(ui->buttonLoop1PumpStart, &QPushButton::clicked, this, &OfflineCooling::pump1Start);
    connect(ui->buttonLoop1PumpStop, &QPushButton::clicked, this, &OfflineCooling::pump1Stop);
    connect(ui->buttonLoop1InletClose, &QPushButton::clicked, this, &OfflineCooling::pump1InletClose);
    connect(ui->buttonLoop1InletOpen, &QPushButton::clicked, this, &OfflineCooling::pump1InletOpen);
    connect(ui->buttonLoop1OutletClose, &QPushButton::clicked, this, &OfflineCooling::pump1OutletClose);
    connect(ui->buttonLoop1OutletOpen, &QPushButton::clicked, this, &OfflineCooling::pump1OutletOpen);

    connect(ui->buttonLoop2PumpStart, &QPushButton::clicked, this, &OfflineCooling::pump2Start);
    connect(ui->buttonLoop2PumpStop, &QPushButton::clicked, this, &OfflineCooling::pump2Stop);
    connect(ui->buttonLoop2InletClose, &QPushButton::clicked, this, &OfflineCooling::pump2InletClose);
    connect(ui->buttonLoop2InletOpen, &QPushButton::clicked, this, &OfflineCooling::pump2InletOpen);
    connect(ui->buttonLoop2OutletClose, &QPushButton::clicked, this, &OfflineCooling::pump2OutletClose);
    connect(ui->buttonLoop2OutletOpen, &QPushButton::clicked, this, &OfflineCooling::pump2OutletOpen);
}

OfflineCooling::~OfflineCooling()
{
    delete ui;
}

void OfflineCooling::refresh()
{
    float fuelTemp1 = m_api->GETCOR(0, 0, 3);
    float fuelTemp2 = m_api->GETCOR(0, 0, 4);

    ui->lineLoop1FuelTemp->setText(QString::number(fuelTemp1, 'f', 1));
    ui->lineLoop2FuelTemp->setText(QString::number(fuelTemp2, 'f', 1));

    float sdc1[3];
    m_api->GETSDC(1, sdc1);

    float sdc2[3];
    m_api->GETSDC(2, sdc2);

    float pump1 = sdc1[0];
    float inlet1 = sdc1[1];
    float outlet1 = sdc1[2];

    float pump2 = sdc2[0];
    float inlet2 = sdc2[1];
    float outlet2 = sdc2[2];

    ui->buttonLoop1PumpStart->setChecked(pump1 == 1.0);
    ui->buttonLoop1PumpStop->setChecked(pump1 == 0.0);
    ui->buttonLoop1InletOpen->setChecked(inlet1 == 1.0);
    ui->buttonLoop1InletClose->setChecked(inlet1 == 0.0);
    ui->buttonLoop1OutletOpen->setChecked(outlet1 == 1.0);
    ui->buttonLoop1OutletClose->setChecked(outlet1 == 0.0);

    ui->buttonLoop2PumpStart->setChecked(pump2 == 1.0);
    ui->buttonLoop2PumpStop->setChecked(pump2 == 0.0);
    ui->buttonLoop2InletOpen->setChecked(inlet2 == 1.0);
    ui->buttonLoop2InletClose->setChecked(inlet2 == 0.0);
    ui->buttonLoop2OutletOpen->setChecked(outlet2 == 1.0);
    ui->buttonLoop2OutletClose->setChecked(outlet2 == 0.0);

    float sdp11[2];
    float sdp12[2];
    float sdp21[2];
    float sdp22[2];

    m_api->GETSDP(1, sdp11, 1);
    m_api->GETSDP(1, sdp12, 2);
    m_api->GETSDP(2, sdp21, 1);
    m_api->GETSDP(2, sdp22, 2);

    ui->lineLoop1CoolingFlow->setText(QString::number(sdp21[0], 'f', 1));
    ui->lineLoop2CoolingFlow->setText(QString::number(sdp22[0], 'f', 1));

    ui->lineLoop1OutletTemp->setText(QString::number(sdp11[0], 'f', 1));
    ui->lineLoop2OutletTemp->setText(QString::number(sdp12[0], 'f', 1));

    ui->lineLoop1DischargeTemp->setText(QString::number(sdp11[1], 'f', 1));
    ui->lineLoop2DischargeTemp->setText(QString::number(sdp12[1], 'f', 1));

    ui->lineLoop1PumpFlow->setText(QString::number(sdp21[1], 'f', 1));
    ui->lineLoop2PumpFlow->setText(QString::number(sdp22[1], 'f', 1));

}

void OfflineCooling::loop1Inc()
{
    m_api->SETFED(0.05, 12);
}

void OfflineCooling::loop1Dec()
{
    m_api->SETFED(-0.05, 12);
}

void OfflineCooling::loop2Inc()
{
    m_api->SETFED(0.05, 13);
}

void OfflineCooling::loop2Dec()
{
    m_api->SETFED(-0.05, 13);
}

void OfflineCooling::pump1Start()
{
    m_api->SETPMP(14, 1);
}

void OfflineCooling::pump1Stop()
{
    m_api->SETPMP(14, 0);
}

void OfflineCooling::pump1InletOpen()
{
    m_api->SETPMP(16, 1);
}

void OfflineCooling::pump1InletClose()
{
    m_api->SETPMP(16, 0);
}

void OfflineCooling::pump1OutletOpen()
{
    m_api->SETPMP(18, 1);
}

void OfflineCooling::pump1OutletClose()
{
    m_api->SETPMP(18, 0);
}

void OfflineCooling::pump2Start()
{
    m_api->SETPMP(15, 1);
}

void OfflineCooling::pump2Stop()
{
    m_api->SETPMP(15, 0);
}

void OfflineCooling::pump2InletOpen()
{
    m_api->SETPMP(17, 1);
}

void OfflineCooling::pump2InletClose()
{
    m_api->SETPMP(17, 0);
}

void OfflineCooling::pump2OutletOpen()
{
    m_api->SETPMP(19, 1);
}

void OfflineCooling::pump2OutletClose()
{
    m_api->SETPMP(19, 0);
}
