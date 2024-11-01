#include "feedwater.h"
#include "ui_feedwater.h"

#include "api.h"

#include <QDebug>

FeedWater::FeedWater(QWidget *parent, Chrnob32API *api) :
    QDialog(parent),
    ui(new Ui::FeedWater), m_api(api)
{
    ui->setupUi(this);

    connect(ui->buttonManual, &QPushButton::clicked, this, &FeedWater::setManualValve);
    connect(ui->buttonAuto, &QPushButton::clicked, this, &FeedWater::setAutoValve);

    connect(ui->buttonStartupValve, &QPushButton::clicked, this, &FeedWater::setStartupValve);
    connect(ui->buttonMainValve, &QPushButton::clicked, this, &FeedWater::setMainValve);

    connect(ui->button3Element, &QPushButton::clicked, this, &FeedWater::toggle3Element);

    connect(ui->buttonFeedPump1, &QPushButton::clicked, this, &FeedWater::toggleFeedPump1);
    connect(ui->buttonFeedPump2, &QPushButton::clicked, this, &FeedWater::toggleFeedPump2);
    connect(ui->buttonFeedPump3, &QPushButton::clicked, this, &FeedWater::toggleFeedPump3);

    connect(ui->buttonFeedInlet1Open, &QPushButton::clicked, this, &FeedWater::feedInlet1Open);
    connect(ui->buttonFeedInlet2Open, &QPushButton::clicked, this, &FeedWater::feedInlet2Open);
    connect(ui->buttonFeedInlet3Open, &QPushButton::clicked, this, &FeedWater::feedInlet3Open);
    connect(ui->buttonFeedInlet1Close, &QPushButton::clicked, this, &FeedWater::feedInlet1Close);
    connect(ui->buttonFeedInlet2Close, &QPushButton::clicked, this, &FeedWater::feedInlet2Close);
    connect(ui->buttonFeedInlet3Close, &QPushButton::clicked, this, &FeedWater::feedInlet3Close);

    connect(ui->buttonFeedDischarge1Open, &QPushButton::clicked, this, &FeedWater::feedOutlet1Open);
    connect(ui->buttonFeedDischarge2Open, &QPushButton::clicked, this, &FeedWater::feedOutlet2Open);
    connect(ui->buttonFeedDischarge3Open, &QPushButton::clicked, this, &FeedWater::feedOutlet3Open);
    connect(ui->buttonFeedDischarge1Close, &QPushButton::clicked, this, &FeedWater::feedOutlet1Close);
    connect(ui->buttonFeedDischarge2Close, &QPushButton::clicked, this, &FeedWater::feedOutlet2Close);
    connect(ui->buttonFeedDischarge3Close, &QPushButton::clicked, this, &FeedWater::feedOutlet3Close);

    connect(ui->buttonDrainAuto, &QPushButton::clicked, this, &FeedWater::drainAuto);
    connect(ui->buttonDrainManual, &QPushButton::clicked, this, &FeedWater::drainManual);
    connect(ui->buttonDrainInc, &QPushButton::clicked, this, &FeedWater::drainInc);
    connect(ui->buttonDrainDec, &QPushButton::clicked, this, &FeedWater::drainDec);

}

FeedWater::~FeedWater()
{
    delete ui;
}

void FeedWater::refresh()
{
    float startupValve = m_api->GETFED(0);
    ui->lineStartupValve->setText(QString::number(startupValve*100, 'f', 0)+"%");

    float feedFlow = m_api->GETFED(1);
    ui->lineFeedFlow->setText(QString::number(feedFlow, 'f', 0));

    float mainValve = m_api->GETFED(10);
    ui->lineMainValve->setText(QString::number(mainValve*100, 'f', 0)+"%");

    float levelSetpoint = m_api->GETFED(7);
    ui->editLevelSetpoint->setText(QString::number(levelSetpoint, 'f', 1));

    int mainStartup = m_api->GETFED(8);
    ui->buttonMainValve->setChecked(mainStartup == 2);
    ui->buttonStartupValve->setChecked(mainStartup == 1);

    int autoManual = m_api->GETFED(11);
    ui->buttonAuto->setChecked(autoManual == 1);
    ui->buttonManual->setChecked(autoManual == 0);

    int control3element = m_api->GETFED(9);
    ui->button3Element->setChecked(control3element == 2);

    float drumPressure = m_api->GETDRM(0);
    ui->lineDrumPressure->setText(QString::number(drumPressure, 'f', 0));

    float drumTemperature = m_api->GETDRM(1);
    ui->lineDrumTemp->setText(QString::number(drumTemperature, 'f', 0));

    float drumLevel = m_api->GETDRM(2);
    ui->lineDrumLevel->setText(QString::number(drumLevel, 'f', 1));

    float valveInlet1 = m_api->GETISO(0, 0);
    ui->lineFeedInlet1->setText(QString::number(valveInlet1*100, 'f', 0)+"%");

    float valveInlet2 = m_api->GETISO(1, 0);
    ui->lineFeedInlet2->setText(QString::number(valveInlet2*100, 'f', 0)+"%");

    float valveInlet3 = m_api->GETISO(2, 0);
    ui->lineFeedInlet3->setText(QString::number(valveInlet3*100, 'f', 0)+"%");

    float valveOutlet1 = m_api->GETISO(0, 1);
    ui->lineFeedDischarge1->setText(QString::number(valveOutlet1*100, 'f', 0)+"%");

    float valveOutlet2 = m_api->GETISO(1, 1);
    ui->lineFeedDischarge2->setText(QString::number(valveOutlet2*100, 'f', 0)+"%");

    float valveOutlet3 = m_api->GETISO(2, 1);
    ui->lineFeedDischarge3->setText(QString::number(valveOutlet3*100, 'f', 0)+"%");

    bool feedPump1 = m_api->GETFED(3) == 1.0; //!FIXME some waiting state needed
    ui->buttonFeedPump1->setChecked(feedPump1);

    bool feedPump2 = m_api->GETFED(4) == 1.0;
    ui->buttonFeedPump2->setChecked(feedPump2);

    bool feedPump3 = m_api->GETFED(5) == 1.0;
    ui->buttonFeedPump3->setChecked(feedPump3);

    float drainValve = m_api->GETFED(14);
    ui->lineDrainValve->setText(QString::number(drainValve*100, 'f', 0)+"%");

    float drainSetpoint = m_api->GETFED(13);
    ui->lineDrainSetpoint->setText(QString::number(drainSetpoint, 'f', 1));

    bool drainAuto = m_api->GETFED(12) == 1.0;
    ui->buttonDrainAuto->setChecked(drainAuto);
    ui->buttonDrainManual->setChecked(!drainAuto);

    float drainFlow = m_api->GETFED(6);
    ui->lineDrainFlow->setText(QString::number(drainFlow, 'f', 1));
}

void FeedWater::toggleFeedPump1()
{
    bool feedPump1 = m_api->GETFED(3) == 1.0;

    m_api->SETPMP(6, !feedPump1);
}

void FeedWater::toggleFeedPump2()
{
    bool feedPump2 = m_api->GETFED(4) == 1.0;

    m_api->SETPMP(7, !feedPump2);
}

void FeedWater::toggleFeedPump3()
{
    bool feedPump3 = m_api->GETFED(5) == 1.0;

    m_api->SETPMP(8, !feedPump3);
}

void FeedWater::feedInlet1Open()
{
    m_api->SETISO(1, 0, 0);
}

void FeedWater::feedInlet2Open()
{
    m_api->SETISO(1, 1, 0);
}

void FeedWater::feedInlet3Open()
{
    m_api->SETISO(1, 2, 0);
}

void FeedWater::feedOutlet1Open()
{
    m_api->SETISO(1, 0, 1);
}

void FeedWater::feedOutlet2Open()
{
    m_api->SETISO(1, 1, 1);
}

void FeedWater::feedOutlet3Open()
{
    m_api->SETISO(1, 2, 1);
}

void FeedWater::feedInlet1Close()
{
    m_api->SETISO(0, 0, 0);
}

void FeedWater::feedInlet2Close()
{
    m_api->SETISO(0, 1, 0);
}

void FeedWater::feedInlet3Close()
{
    m_api->SETISO(0, 2, 0);
}

void FeedWater::feedOutlet1Close()
{
    m_api->SETISO(0, 0, 1);
}

void FeedWater::feedOutlet2Close()
{
    m_api->SETISO(0, 1, 1);
}

void FeedWater::feedOutlet3Close()
{
    m_api->SETISO(0, 2, 1);
}

void FeedWater::setManualValve()
{
    m_api->SETFED(0.0, 0);
}

void FeedWater::setAutoValve()
{
    m_api->SETFED(0.0, 1);
}

void FeedWater::toggle3Element()
{
    int control3element = m_api->GETFED(9);
    if(control3element == 2) // 3 element active
    {
        m_api->SETFED(0.0, 6);
    }
    else
    {
        m_api->SETFED(0.0, 7);
    }
}

void FeedWater::setStartupValve()
{
    m_api->SETFED(0.0, 4);
}

void FeedWater::setMainValve()
{
    m_api->SETFED(0.0, 5);
}

void FeedWater::drainAuto()
{
    m_api->SETFED(0.0, 9);
}

void FeedWater::drainManual()
{
    m_api->SETFED(0.0, 8);
}

void FeedWater::drainInc()
{
    m_api->SETFED(0.005, 11);
}

void FeedWater::drainDec()
{
    m_api->SETFED(-0.005, 11);
}
