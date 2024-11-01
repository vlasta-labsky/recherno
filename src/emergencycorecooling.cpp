#include "emergencycorecooling.h"
#include "ui_emergencycorecooling.h"

#include "api.h"

#include <QDebug>

EmergencyCoreCooling::EmergencyCoreCooling(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::EmergencyCoreCooling)
{
    ui->setupUi(this);

    connect(ui->buttonValveClose, &QPushButton::clicked, this, &EmergencyCoreCooling::valveClose);
    connect(ui->buttonValveOpen, &QPushButton::clicked, this, &EmergencyCoreCooling::valveOpen);
    connect(ui->buttonValveAuto, &QPushButton::clicked, this, &EmergencyCoreCooling::valveAuto);

    connect(ui->buttonPump1Start, &QPushButton::clicked, this, &EmergencyCoreCooling::pump1Start);
    connect(ui->buttonPump1Stop, &QPushButton::clicked, this, &EmergencyCoreCooling::pump1Stop);
    connect(ui->buttonPump2Start, &QPushButton::clicked, this, &EmergencyCoreCooling::pump2Start);
    connect(ui->buttonPump2Stop, &QPushButton::clicked, this, &EmergencyCoreCooling::pump2Stop);

    connect(ui->buttonDieselStart, &QPushButton::clicked, this, &EmergencyCoreCooling::dieselStart);
    connect(ui->buttonDieselStop, &QPushButton::clicked, this, &EmergencyCoreCooling::dieselStop);

    connect(ui->buttonDieselInc, &QPushButton::clicked, this, &EmergencyCoreCooling::dieselInc);
    connect(ui->buttonDieselDec, &QPushButton::clicked, this, &EmergencyCoreCooling::dieselDec);
}

EmergencyCoreCooling::~EmergencyCoreCooling()
{
    delete ui;
}

void EmergencyCoreCooling::refresh()
{
    float ecc[10];
    m_api->GETECC(4, 0, 0, ecc, sizeof(ecc));

    float eccValveMode = ecc[8];
    ui->buttonValveAuto->setChecked(eccValveMode == -1.0);
    ui->buttonValveClose->setChecked(eccValveMode == 0.0);
    ui->buttonValveOpen->setChecked(eccValveMode == 1.0);

    float eccPump1 = ecc[5];
    ui->buttonPump1Start->setChecked(eccPump1 == 1.0);
    ui->buttonPump1Stop->setChecked(eccPump1 == 0.0);

    float eccPump2 = ecc[6];
    ui->buttonPump2Start->setChecked(eccPump2 == 1.0);
    ui->buttonPump2Stop->setChecked(eccPump2 == 0.0);

    float dieselPump = ecc[7];
    ui->buttonDieselStart->setChecked(dieselPump == 1.0);
    ui->buttonDieselStop->setChecked(dieselPump == 0.0);

    float valveFlow = ecc[0];
    ui->lineValveFlow->setText(QString::number(valveFlow, 'f', 1));

    float pump1Flow = ecc[1];
    ui->linePump1Flow->setText(QString::number(pump1Flow, 'f', 1));

    float pump2Flow = ecc[2];
    ui->linePump2Flow->setText(QString::number(pump2Flow, 'f', 1));

    float eccValveState1 = ecc[4];
    ui->lineValve1->setText(QString::number(eccValveState1*100, 'f', 0)+"%");

    float eccValveState2 = ecc[9];
    ui->lineValve2->setText(QString::number(eccValveState2*100, 'f', 0)+"%");
}

void EmergencyCoreCooling::valveClose()
{
    m_api->SETPMP(23, 0);
}

void EmergencyCoreCooling::valveOpen()
{
    m_api->SETPMP(23, 1);
}

void EmergencyCoreCooling::valveAuto()
{
    m_api->SETPMP(23, -1);
}

void EmergencyCoreCooling::pump1Start()
{
    m_api->SETPMP(20, 1);
}

void EmergencyCoreCooling::pump1Stop()
{
    m_api->SETPMP(20, 0);
}

void EmergencyCoreCooling::pump2Start()
{
    m_api->SETPMP(21, 1);
}

void EmergencyCoreCooling::pump2Stop()
{
    m_api->SETPMP(21, 0);
}

void EmergencyCoreCooling::dieselStart()
{
    m_api->SETPMP(22, 1);
}

void EmergencyCoreCooling::dieselStop()
{
    m_api->SETPMP(22, 0);
}

void EmergencyCoreCooling::dieselInc()
{

}

void EmergencyCoreCooling::dieselDec()
{

}
