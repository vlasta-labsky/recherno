#include "deareator.h"
#include "ui_deareator.h"

#include "api.h"

Deareator::Deareator(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::Deareator)
{
    ui->setupUi(this);

    connect(ui->buttonLevelSetpointInc, &QPushButton::clicked, this, &Deareator::levelSetpointInc);
    connect(ui->buttonLevelSetpointDec, &QPushButton::clicked, this, &Deareator::levelSetpointDec);
    connect(ui->buttonFlowInc, &QPushButton::clicked, this, &Deareator::flowValveInc);
    connect(ui->buttonFlowDec, &QPushButton::clicked, this, &Deareator::flowValveDec);
    connect(ui->buttonDrainInc, &QPushButton::clicked, this, &Deareator::drainValveInc);
    connect(ui->buttonDrainDec, &QPushButton::clicked, this, &Deareator::drainValveDec);

    connect(ui->buttonLevelAuto, &QPushButton::clicked, this, &Deareator::levelAuto);
    connect(ui->buttonLevelManual, &QPushButton::clicked, this, &Deareator::levelManual);
}

Deareator::~Deareator()
{
    delete ui;
}

void Deareator::refresh()
{
    const auto [daLevel, manualAuto] = m_api->GETDAR(3);
    ui->lineDALevel->setText(QString::number(daLevel, 'f', 1));

    ui->buttonLevelAuto->setChecked(manualAuto);
    ui->buttonLevelManual->setChecked(!manualAuto);

    float hotwellLevel = m_api->GETCND(34);
    ui->lineHotwellLevel->setText(QString::number(hotwellLevel, 'f', 1));

    float drainValve = std::get<0>(m_api->GETDAR(13));
    ui->lineDrainValve->setText(QString::number(drainValve * 100, 'f', 1)+"%");

    float flowValve = std::get<0>(m_api->GETDAR(32));
    ui->lineFlowValve->setText(QString::number(flowValve * 100, 'f', 1)+"%");

    float levelSetpoint = std::get<0>(m_api->GETDAR(1));
    ui->lineLevelSetpoint->setText(QString::number(levelSetpoint, 'f', 1));

}

void Deareator::levelSetpointInc()
{
    float levelSetpoint = std::get<0>(m_api->GETDAR(1));

    m_api->SETDAR(levelSetpoint+0.5, 3);
}

void Deareator::levelSetpointDec()
{
    float levelSetpoint = std::get<0>(m_api->GETDAR(1));

    m_api->SETDAR(levelSetpoint-0.5, 3);
}

void Deareator::flowValveInc()
{
    m_api->SETDAR(0.02, 15);
}

void Deareator::flowValveDec()
{
    m_api->SETDAR(-0.02, 15);
}

void Deareator::drainValveInc()
{
    m_api->SETDAR(0.005, 8);
}

void Deareator::drainValveDec()
{
    m_api->SETDAR(-0.005, 8);
}

void Deareator::levelManual()
{
    m_api->SETDAR(0.0, 1);
}

void Deareator::levelAuto()
{
    m_api->SETDAR(1.0, 1);
}
