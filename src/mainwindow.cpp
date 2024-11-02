#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "systemsummary.h"
#include "feedwater.h"
#include "loop1.h"
#include "loop2.h"
#include "condenser.h"
#include "deareator.h"
#include "reactor.h"
#include "emergencycorecooling.h"
#include "offlinecooling.h"
#include "reactorcontrolrods.h"
#include "turbinesupport.h"
#include "turbine.h"

#include "virtualmachine.h"

#include "impl.h"
#include "api.h"

#include <QFile>

#include <QPushButton>

#define CHRNOB32_INTERVAL 100
#define CHRNOB32_DLL "chrnob_32.dll"

#define ICD_FILE "shutdown.icd"
//#define ICD_FILE "1800PSIG_660MW.ICD"
//#define ICD_FILE "chernobyl04.icd"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_running(false)
{
    ui->setupUi(this);

    m_machine = new VirtualMachine;
    m_machine->init();

    m_machine->implement("msvcrt.dll:fputc", &impl_msvcrt_fputc);
    m_machine->implement("msvcrt.dll:_lock", &impl_msvcrt_lock);
    m_machine->implement("msvcrt.dll:_unlock", &impl_msvcrt_unlock);
    m_machine->implement("msvcrt.dll:calloc", &impl_msvcrt_calloc);
    m_machine->implement("msvcrt.dll:malloc", &impl_msvcrt_malloc);
    m_machine->implement("msvcrt.dll:_errno", &impl_msvcrt_errno);
    m_machine->implement("msvcrt.dll:_initterm", &impl_msvcrt_initterm);

    m_machine->implement("kernel32.dll:GetModuleHandleA", &impl_kernel32_GetModuleHandleA);
    m_machine->implement("kernel32.dll:SetUnhandledExceptionFilter", &impl_kernel32_SetUnhandledExceptionFilter);

    m_machine->implement("dforrtd.dll:_FIexp", &impl_ciexp); // it is in fact msvcrtd.dll:_CIexp
    m_machine->implement("dforrtd.dll:_FIsqrt", &impl_cisqrt); // it is in fact msvcrtd.dll:_CIsqrt

    if(!m_machine->load(CHRNOB32_DLL))
    {
        qDebug() << "failed to load " CHRNOB32_DLL  ", exiting";

        exit(1);
    }

    m_api = new Chrnob32API(m_machine);

    restart();

    m_timer = new QTimer(this);
    m_timer->setInterval(CHRNOB32_INTERVAL);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::tick);
    m_timer->start();

    connect(ui->buttonStart, &QPushButton::clicked, this, &MainWindow::start);
    connect(ui->buttonStop, &QPushButton::clicked, this, &MainWindow::stop);
    connect(ui->buttonRestart, &QPushButton::clicked, this, &MainWindow::restart);

    connect(ui->buttonAlarmsReset, &QPushButton::clicked, this, &MainWindow::resetAlarms);

    m_systemSummary = new SystemSummary(this, m_api);
    m_feedWater = new FeedWater(this, m_api);
    m_loop1 = new Loop1(this, m_api);
    m_loop2 = new Loop2(this, m_api);
    m_condenser = new Condenser(this, m_api);
    m_deareator = new Deareator(this, m_api);
    m_reactor = new Reactor(this, m_api);
    m_ecc = new EmergencyCoreCooling(this, m_api);
    m_offline = new OfflineCooling(this, m_api);
    m_rods = new ReactorControlRods(this, m_api);
    m_turbineSupport = new TurbineSupport(this, m_api);
    m_turbine = new Turbine(this, m_api);

    connect(ui->buttonSystemSummary, &QPushButton::clicked, this, &MainWindow::systemSummary);
    connect(ui->buttonFeedWater, &QPushButton::clicked, this, &MainWindow::feedWater);
    connect(ui->buttonLoop1, &QPushButton::clicked, this, &MainWindow::loop1);
    connect(ui->buttonLoop2, &QPushButton::clicked, this, &MainWindow::loop2);
    connect(ui->buttonCondenser, &QPushButton::clicked, this, &MainWindow::condenser);
    connect(ui->buttonDeareator, &QPushButton::clicked, this, &MainWindow::deareator);
    connect(ui->buttonReactor, &QPushButton::clicked, this, &MainWindow::reactor);
    connect(ui->buttonECC, &QPushButton::clicked, this, &MainWindow::emergencyCoreCooling);
    connect(ui->buttonOfflineCooling, &QPushButton::clicked, this, &MainWindow::offlineCooling);
    connect(ui->buttonControlRods, &QPushButton::clicked, this, &MainWindow::controlRods);
    connect(ui->buttonTurbineSupport, &QPushButton::clicked, this, &MainWindow::turbineSupport);
    connect(ui->buttonTurbine, &QPushButton::clicked, this, &MainWindow::turbine);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    m_running = true;
}

void MainWindow::stop()
{
    m_running = false;
}

void MainWindow::restart()
{
    uint32_t icdSize = m_api->GET_DATA_CNT(4);

    qDebug() << "icd data size" << Qt::hex << 4*icdSize;

    QFile icdFile(ICD_FILE);
    if(!icdFile.open(QIODevice::ReadOnly))
    {
        qCritical() << "icd file" << icdFile.fileName() << "not found, exiting";
        exit(1);
    }

    QByteArray icdFileContents = icdFile.readAll();

    qDebug() << "icd file size" << Qt::hex << icdFileContents.size();

    if(4*icdSize != icdFileContents.size())
    {
        qCritical() << "icd file" << icdFile.fileName() << "invalid size, exiting";
        exit(1);
    }

    m_api->SET_IC_DATA(icdFileContents.data(), icdSize);

    resetAlarms();
}

void MainWindow::systemSummary()
{
    m_systemSummary->show();
}

void MainWindow::feedWater()
{
    m_feedWater->show();
}

void MainWindow::loop1()
{
    m_loop1->show();
}

void MainWindow::loop2()
{
    m_loop2->show();
}

void MainWindow::condenser()
{
    m_condenser->show();
}

void MainWindow::deareator()
{
    m_deareator->show();
}

void MainWindow::reactor()
{
    m_reactor->show();
}

void MainWindow::emergencyCoreCooling()
{
    m_ecc->show();
}

void MainWindow::offlineCooling()
{
    m_offline->show();
}

void MainWindow::controlRods()
{
    m_rods->show();
}

void MainWindow::turbineSupport()
{
    m_turbineSupport->show();
}

void MainWindow::turbine()
{
    m_turbine->show();
}

void MainWindow::resetAlarms()
{
    m_api->ALARST(0, 0);
}

void MainWindow::tick()
{
    //qDebug() << "tick !!!";

   QList<uint16_t> fedIds = {6, 12, 13, 14};


//    qDebug() << "------";
//    for(auto id : fedIds)
//    {
//        float val = m_api->GETFED(id);

//        qDebug() << "GETFED" << id << val;
//    }
//    m_api->SETPMP(6, 1);

//    qDebug() << "after";
//    for(auto id : fedIds)
//    {
//        float val = m_api->GETFED(id);

//        qDebug() << "GETFED" << id << val;
//    }
//    m_api->SETPMP(6, 0);

//    qDebug() << "after";
//    for(auto id : fedIds)
//    {
//        float val = m_api->GETFED(id);

//        qDebug() << "GETFED" << id << val;
//    }

//    QList<uint16_t> drmIds = {0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc};

//    for(auto id : drmIds)
//    {
//        float val = m_api->GETDRM(id);

//        qDebug() << "GETDRM" << id << val;
//    }

//   qDebug() << "asd";
//   QList<uint16_t> trbIds = {0x0, 0x4, 0x5, 0x6, 0x8, 0x15, 0x1, 0x2, 0x3, 0x29, 0x10, 0x9, 0xf};
//   for(auto id : trbIds)
//   {
//        const auto [val1, val2] = m_api->GETTRB(id);

//        qDebug() << "GETTRB" << id << val1 << val2;
//   }

//    QList<uint16_t> cndIds = {0x1c, 0xc, 0xd, 0xe};
//    for(auto id : cndIds)
//    {
//        float val = m_api->GETCND(id);

//        qDebug() << "GETCND" << id << val;
//    }

//    QList<uint16_t> darIds = {2, 5, 7, 9, 10, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13,
//                              0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1f, 0x20};

//    QList<uint16_t> darIds = {0x20};

//    for(auto id : darIds)
//    {
//        float val = m_api->GETDAR(id);

//        qDebug() << "GETDAR" << id << val;
//    }

//   QVector<uint16_t> ixcIds = {3, 0xd, 4, 0xe, 1, 2, 7, 0xf};
//   for(auto id : ixcIds)
//   {
//        float ret = m_api->IXCLGT(id);

//        qDebug() << "ixclgt" << id << ret;
//   }

    ui->labelRunning->setText(m_running ? "Running" : "Stopped");

//    m_api->SETPMP(6, 1);

    if(m_running)
    {
        m_api->REACTRX((float)(CHRNOB32_INTERVAL)/1000.0);
    }

    m_api->DOALAR();

    QString alarmText;
    for(int i = 1; i < 99; i++)
    {
        QString alarm = m_api->ALARMS_GET(i);
        if(alarm.size())
            alarmText += alarm + "<br>";
    }

    ui->textAlarms->setText(alarmText);

    m_systemSummary->refresh();
    m_feedWater->refresh();
    m_loop1->refresh();
    m_loop2->refresh();
    m_condenser->refresh();
    m_deareator->refresh();
    m_reactor->refresh();
    m_ecc->refresh();
    m_offline->refresh();
    m_rods->refresh();
    m_turbineSupport->refresh();
    m_turbine->refresh();
}
