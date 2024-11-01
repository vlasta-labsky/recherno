#include "loop2.h"
#include "ui_loop2.h"

#include "api.h"

Loop2::Loop2(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::Loop2)
{
    ui->setupUi(this);

    connect(ui->buttonLoop2Pump1, &QPushButton::clicked, this, &Loop2::toggleLoop2Pump1);
    connect(ui->buttonLoop2Pump2, &QPushButton::clicked, this, &Loop2::toggleLoop2Pump2);
    connect(ui->buttonLoop2Pump3, &QPushButton::clicked, this, &Loop2::toggleLoop2Pump3);

    connect(ui->buttonLoop2Inlet1Open, &QPushButton::clicked, this, &Loop2::loop2Inlet1Open);
    connect(ui->buttonLoop2Inlet2Open, &QPushButton::clicked, this, &Loop2::loop2Inlet2Open);
    connect(ui->buttonLoop2Inlet3Open, &QPushButton::clicked, this, &Loop2::loop2Inlet3Open);
    connect(ui->buttonLoop2Inlet1Close, &QPushButton::clicked, this, &Loop2::loop2Inlet1Close);
    connect(ui->buttonLoop2Inlet2Close, &QPushButton::clicked, this, &Loop2::loop2Inlet2Close);
    connect(ui->buttonLoop2Inlet3Close, &QPushButton::clicked, this, &Loop2::loop2Inlet3Close);
    connect(ui->buttonLoop2Outlet1Open, &QPushButton::clicked, this, &Loop2::loop2Outlet1Open);
    connect(ui->buttonLoop2Outlet2Open, &QPushButton::clicked, this, &Loop2::loop2Outlet2Open);
    connect(ui->buttonLoop2Outlet3Open, &QPushButton::clicked, this, &Loop2::loop2Outlet3Open);
    connect(ui->buttonLoop2Outlet1Close, &QPushButton::clicked, this, &Loop2::loop2Outlet1Close);
    connect(ui->buttonLoop2Outlet2Close, &QPushButton::clicked, this, &Loop2::loop2Outlet2Close);
    connect(ui->buttonLoop2Outlet3Close, &QPushButton::clicked, this, &Loop2::loop2Outlet3Close);
}

Loop2::~Loop2()
{
    delete ui;
}

void Loop2::refresh()
{
    float valves[12];
    m_api->GETVLV(valves);

    ui->lineLoop2Inlet1->setText(QString::number(valves[3]*100, 'f', 0)+"%");
    ui->lineLoop2Inlet2->setText(QString::number(valves[4]*100, 'f', 0)+"%");
    ui->lineLoop2Inlet3->setText(QString::number(valves[5]*100, 'f', 0)+"%");

    ui->lineLoop2Outlet1->setText(QString::number(valves[9]*100, 'f', 0)+"%");
    ui->lineLoop2Outlet2->setText(QString::number(valves[10]*100, 'f', 0)+"%");
    ui->lineLoop2Outlet3->setText(QString::number(valves[11]*100, 'f', 0)+"%");

    uint16_t pumps[6];
    float flow[2];
    m_api->GETPMP(pumps, flow);

    ui->buttonLoop2Pump1->setChecked(pumps[3]);
    ui->buttonLoop2Pump2->setChecked(pumps[4]);
    ui->buttonLoop2Pump3->setChecked(pumps[5]);

    ui->lineLoop2Flow->setText(QString::number(flow[1], 'f', 0));
}

void Loop2::toggleLoop2Pump1()
{
    uint16_t pumps[6];
    float flow[2];

    m_api->GETPMP(pumps, flow);

    m_api->SETPMP(3, !pumps[3]);
}

void Loop2::toggleLoop2Pump2()
{
    uint16_t pumps[6];
    float flow[2];

    m_api->GETPMP(pumps, flow);

    m_api->SETPMP(4, !pumps[4]);
}

void Loop2::toggleLoop2Pump3()
{
    uint16_t pumps[6];
    float flow[2];

    m_api->GETPMP(pumps, flow);

    m_api->SETPMP(5, !pumps[5]);
}

void Loop2::loop2Inlet1Open()
{
    m_api->SETINL(3, 1);
}

void Loop2::loop2Inlet2Open()
{
    m_api->SETINL(4, 1);
}

void Loop2::loop2Inlet3Open()
{
    m_api->SETINL(5, 1);
}

void Loop2::loop2Inlet1Close()
{
    m_api->SETINL(3, 0);
}

void Loop2::loop2Inlet2Close()
{
    m_api->SETINL(4, 0);
}

void Loop2::loop2Inlet3Close()
{
    m_api->SETINL(5, 0);
}

void Loop2::loop2Outlet1Open()
{
    m_api->SETDIS(3, 1);
}

void Loop2::loop2Outlet2Open()
{
    m_api->SETDIS(4, 1);
}

void Loop2::loop2Outlet3Open()
{
    m_api->SETDIS(5, 1);
}

void Loop2::loop2Outlet1Close()
{
    m_api->SETDIS(3, 0);
}

void Loop2::loop2Outlet2Close()
{
    m_api->SETDIS(4, 0);
}

void Loop2::loop2Outlet3Close()
{
    m_api->SETDIS(5, 0);
}
