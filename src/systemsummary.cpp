#include "systemsummary.h"
#include "ui_systemsummary.h"

#include "api.h"

#include <QDebug>

SystemSummary::SystemSummary(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::SystemSummary)
{
    ui->setupUi(this);
}

SystemSummary::~SystemSummary()
{
    delete ui;
}

void SystemSummary::refresh()
{
    float turbineRpm = std::get<0>(m_api->GETTRB(9));
    ui->lineTurbineRPM->setText(QString::number(turbineRpm, 'f', 0));

    float turbinePower = std::get<0>(m_api->GETTRB(15));
    ui->lineOutputPower->setText(QString::number(turbinePower, 'f', 1)+"MW");

    float turbineDiffExp = std::get<0>(m_api->GETTRB(16));
    ui->lineTurbineDiffExp->setText(QString::number(turbineDiffExp, 'f', 3));

    float feedWaterFlow = m_api->GETFED(1);
    ui->lineFeedFlow->setText(QString::number(feedWaterFlow, 'f', 0));

    float drumPressure = m_api->GETDRM(0);
    ui->lineDrumPressure->setText(QString::number(drumPressure, 'f', 0)+"kPa");

    float drumLevel = m_api->GETDRM(2);
    ui->lineDrumLevel->setText(QString::number(drumLevel, 'f', 1));

    float daLevel = std::get<0>(m_api->GETDAR(3));
    ui->lineDaLevel->setText(QString::number(daLevel, 'f', 1));

    float hotwellLevel = m_api->GETCND(34);
    ui->lineHotwellLevel->setText(QString::number(hotwellLevel, 'f', 1));

    float condenserVacuum = 101.3 - m_api->GETCND(0);
    ui->lineCondenserVacuum->setText(QString::number(condenserVacuum, 'f', 1)+"kPa");

    float voiding1 = 1.0 - m_api->GETCOR(0, 0, 5);
    float voiding2 = 1.0 - m_api->GETCOR(0, 0, 6);

    ui->lineVoidLoop1->setText(QString::number(voiding1*100, 'f', 1)+"%");
    ui->lineVoidLoop2->setText(QString::number(voiding2*100, 'f', 1)+"%");

    float ecc[10];
    m_api->GETECC(4, 0, 0, ecc, sizeof(ecc));

    QString eccStatus = ecc[8] == 0.0 ? "Blocked"
                      : ecc[8] == -1.0 ? "Auto"
                      : ecc[8] == 1.0 ? "Open" : "";

    ui->lineECCStatus->setText(eccStatus);

    float thermal[25];
    m_api->GETTHR(thermal);

    float averageThermal = 0.0;
    for(int i = 0; i < 25; i++)
    {
        averageThermal += thermal[i];
    }
    averageThermal /= 25;

    ui->lineReactorPower->setText(QString::number(averageThermal*100, 'f', 1)+"%");
}
