#include "loop1.h"
#include "ui_loop1.h"

#include "api.h"

Loop1::Loop1(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::Loop1)
{
    ui->setupUi(this);

    connect(ui->buttonLoop1Pump1, &QPushButton::clicked, this, &Loop1::toggleLoop1Pump1);
    connect(ui->buttonLoop1Pump2, &QPushButton::clicked, this, &Loop1::toggleLoop1Pump2);
    connect(ui->buttonLoop1Pump3, &QPushButton::clicked, this, &Loop1::toggleLoop1Pump3);

    connect(ui->buttonLoop1Inlet1Open, &QPushButton::clicked, this, &Loop1::loop1Inlet1Open);
    connect(ui->buttonLoop1Inlet2Open, &QPushButton::clicked, this, &Loop1::loop1Inlet2Open);
    connect(ui->buttonLoop1Inlet3Open, &QPushButton::clicked, this, &Loop1::loop1Inlet3Open);
    connect(ui->buttonLoop1Inlet1Close, &QPushButton::clicked, this, &Loop1::loop1Inlet1Close);
    connect(ui->buttonLoop1Inlet2Close, &QPushButton::clicked, this, &Loop1::loop1Inlet2Close);
    connect(ui->buttonLoop1Inlet3Close, &QPushButton::clicked, this, &Loop1::loop1Inlet3Close);
    connect(ui->buttonLoop1Outlet1Open, &QPushButton::clicked, this, &Loop1::loop1Outlet1Open);
    connect(ui->buttonLoop1Outlet2Open, &QPushButton::clicked, this, &Loop1::loop1Outlet2Open);
    connect(ui->buttonLoop1Outlet3Open, &QPushButton::clicked, this, &Loop1::loop1Outlet3Open);
    connect(ui->buttonLoop1Outlet1Close, &QPushButton::clicked, this, &Loop1::loop1Outlet1Close);
    connect(ui->buttonLoop1Outlet2Close, &QPushButton::clicked, this, &Loop1::loop1Outlet2Close);
    connect(ui->buttonLoop1Outlet3Close, &QPushButton::clicked, this, &Loop1::loop1Outlet3Close);
}

Loop1::~Loop1()
{
    delete ui;
}

void Loop1::refresh()
{
    float valves[12];
    m_api->GETVLV(valves);

    ui->lineLoop1Inlet1->setText(QString::number(valves[0]*100, 'f', 0)+"%");
    ui->lineLoop1Inlet2->setText(QString::number(valves[1]*100, 'f', 0)+"%");
    ui->lineLoop1Inlet3->setText(QString::number(valves[2]*100, 'f', 0)+"%");

    ui->lineLoop1Outlet1->setText(QString::number(valves[6]*100, 'f', 0)+"%");
    ui->lineLoop1Outlet2->setText(QString::number(valves[7]*100, 'f', 0)+"%");
    ui->lineLoop1Outlet3->setText(QString::number(valves[8]*100, 'f', 0)+"%");

    uint16_t pumps[6];
    float flow[2];
    m_api->GETPMP(pumps, flow);

    ui->buttonLoop1Pump1->setChecked(pumps[0]);
    ui->buttonLoop1Pump2->setChecked(pumps[1]);
    ui->buttonLoop1Pump3->setChecked(pumps[2]);

    ui->lineLoop1Flow->setText(QString::number(flow[0], 'f', 0));
}

void Loop1::toggleLoop1Pump1()
{
    uint16_t pumps[6];
    float flow[2];

    m_api->GETPMP(pumps, flow);

    m_api->SETPMP(0, !pumps[0]);
}

void Loop1::toggleLoop1Pump2()
{
    uint16_t pumps[6];
    float flow[2];

    m_api->GETPMP(pumps, flow);

    m_api->SETPMP(1, !pumps[1]);
}

void Loop1::toggleLoop1Pump3()
{
    uint16_t pumps[6];
    float flow[2];

    m_api->GETPMP(pumps, flow);

    m_api->SETPMP(2, !pumps[2]);
}

void Loop1::loop1Inlet1Open()
{
    m_api->SETINL(0, 1);
}

void Loop1::loop1Inlet2Open()
{
    m_api->SETINL(1, 1);
}

void Loop1::loop1Inlet3Open()
{
    m_api->SETINL(2, 1);
}

void Loop1::loop1Inlet1Close()
{
    m_api->SETINL(0, 0);
}

void Loop1::loop1Inlet2Close()
{
    m_api->SETINL(1, 0);
}

void Loop1::loop1Inlet3Close()
{
    m_api->SETINL(2, 0);
}

void Loop1::loop1Outlet1Open()
{
    m_api->SETDIS(0, 1);
}

void Loop1::loop1Outlet2Open()
{
    m_api->SETDIS(1, 1);
}

void Loop1::loop1Outlet3Open()
{
    m_api->SETDIS(2, 1);
}

void Loop1::loop1Outlet1Close()
{
    m_api->SETDIS(0, 0);
}

void Loop1::loop1Outlet2Close()
{
    m_api->SETDIS(1, 0);
}

void Loop1::loop1Outlet3Close()
{
    m_api->SETDIS(2, 0);
}
