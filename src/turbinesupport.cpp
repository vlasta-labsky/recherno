#include "turbinesupport.h"
#include "ui_turbinesupport.h"

#include "api.h"

#include <QDebug>

TurbineSupport::TurbineSupport(QWidget *parent, Chrnob32API *api) :
    QDialog(parent),
    ui(new Ui::TurbineSupport),
    m_api(api)
{
    ui->setupUi(this);

    connect(ui->buttonHydraulicStart, &QPushButton::clicked, this, &TurbineSupport::hydraulicStart);
    connect(ui->buttonHydraulicStop, &QPushButton::clicked, this, &TurbineSupport::hydraulicStop);

    connect(ui->buttonLubeStart, &QPushButton::clicked, this, &TurbineSupport::lubeStart);
    connect(ui->buttonLubeStop, &QPushButton::clicked, this, &TurbineSupport::lubeStop);

    connect(ui->buttonSteamSealStart, &QPushButton::clicked, this, &TurbineSupport::steamSealStart);
    connect(ui->buttonSteamSealStop, &QPushButton::clicked, this, &TurbineSupport::steamSealStop);

    connect(ui->buttonSteamDrainOpen, &QPushButton::clicked, this, &TurbineSupport::steamDrainOpen);
    connect(ui->buttonSteamDrainClose, &QPushButton::clicked, this, &TurbineSupport::steamDrainClose);
}

TurbineSupport::~TurbineSupport()
{
    delete ui;
}

void TurbineSupport::refresh()
{
    int lubeOil = m_api->GETTRB(34);
    int hydraulicOil = m_api->GETTRB(33);
    int steamSeal = m_api->GETTRB(35);
    int steamDrain = m_api->GETTRB(36);

    float lubeOilPressure = m_api->GETTRB(38) - 101.3;
    float hydraulicOilPressure = m_api->GETTRB(37) - 101.3;
    float steamSealPressure = m_api->GETTRB(39) - 101.3;

    ui->buttonLubeStop->setChecked(lubeOil == 0);
    ui->buttonLubeStart->setChecked(lubeOil == 1);

    ui->buttonHydraulicStop->setChecked(hydraulicOil == 0);
    ui->buttonHydraulicStart->setChecked(hydraulicOil == 1);

    ui->buttonSteamSealStop->setChecked(steamSeal == 0);
    ui->buttonSteamSealStart->setChecked(steamSeal == 1);

    ui->buttonSteamDrainClose->setChecked(steamDrain == 0);
    ui->buttonSteamDrainOpen->setChecked(steamDrain == 1);

    ui->lineLube->setText(QString::number(lubeOilPressure, 'f', 1)+"kPa");
    ui->lineHydraulic->setText(QString::number(hydraulicOilPressure, 'f', 1)+"kPa");
    ui->lineSteamSeal->setText(QString::number(steamSealPressure, 'f', 1)+"kPa");
}

void TurbineSupport::lubeStart()
{
    m_api->SETTRB(1, 13);
}

void TurbineSupport::lubeStop()
{
    m_api->SETTRB(0, 13);
}

void TurbineSupport::hydraulicStart()
{
    m_api->SETTRB(1, 12);
}

void TurbineSupport::hydraulicStop()
{
    m_api->SETTRB(0, 12);
}

void TurbineSupport::steamSealStart()
{
    m_api->SETTRB(1, 14);
}

void TurbineSupport::steamSealStop()
{
    m_api->SETTRB(0, 14);
}

void TurbineSupport::steamDrainOpen()
{
    m_api->SETTRB(1, 15);
}

void TurbineSupport::steamDrainClose()
{
    m_api->SETTRB(0, 15);
}
