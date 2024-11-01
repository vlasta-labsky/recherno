#include "reactorcontrolrods.h"
#include "ui_reactorcontrolrods.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

#include "api.h"

#include <QDebug>

ReactorControlRods::ReactorControlRods(QWidget *parent, Chrnob32API *api) :
    QDialog(parent), m_api(api),
    ui(new Ui::ReactorControlRods)
{
    ui->setupUi(this);

    connect(ui->buttonPull, &QPushButton::clicked, this, &ReactorControlRods::pullRods);
    connect(ui->buttonHold, &QPushButton::clicked, this, &ReactorControlRods::holdRods);
    connect(ui->buttonInsert, &QPushButton::clicked, this, &ReactorControlRods::insertRods);

    connect(ui->buttonCenter, &QPushButton::clicked, this, &ReactorControlRods::centerOnly);

    connect(ui->buttonFast, &QPushButton::clicked, this, &ReactorControlRods::speedFast);
    connect(ui->buttonMedium, &QPushButton::clicked, this, &ReactorControlRods::speedMedium);
    connect(ui->buttonSlow, &QPushButton::clicked, this, &ReactorControlRods::speedSlow);

    QGridLayout *rodLayout = new QGridLayout;

    for(int x = 0; x < 5; x++)
    {
        for(int y = 0; y < 5; y++)
        {
            int id = x + 5*y;
            QGridLayout *singleRodLayout = new QGridLayout;
            singleRodLayout->setHorizontalSpacing(0);
            singleRodLayout->setVerticalSpacing(0);

            m_positions[id] = new QLineEdit();
            m_temps[id] = new QLineEdit();
            m_thermal[id] = new QLineEdit();

            singleRodLayout->addWidget(m_positions[id], 0, 0, 2, 1);
            singleRodLayout->addWidget(m_temps[id], 2, 0);
            singleRodLayout->addWidget(m_thermal[id], 3, 0);

            QPushButton *inc = new QPushButton("+");
            inc->setMaximumWidth(12);
            inc->setMaximumHeight(12);

            QPushButton *dec = new QPushButton("-");
            dec->setMaximumWidth(12);
            dec->setMaximumHeight(12);

            singleRodLayout->addWidget(inc, 0, 1);
            singleRodLayout->addWidget(dec, 1, 1);

            connect(inc, &QPushButton::clicked, this, [x, y, this]()
            {
                rodInc(x, y);
            });

            connect(dec, &QPushButton::clicked, this, [x, y, this]()
            {
                rodDec(x, y);
            });


            rodLayout->addLayout(singleRodLayout, y, x);
        }
    }

    ui->groupRods->setLayout(rodLayout);
}

ReactorControlRods::~ReactorControlRods()
{
    delete ui;
}

void ReactorControlRods::refresh()
{
    int rodSpeed = m_api->GETMOV(1);
    int centerOnly = m_api->GETMOV(2);

    ui->buttonSlow->setChecked(rodSpeed == 1);
    ui->buttonMedium->setChecked(rodSpeed == 2);
    ui->buttonFast->setChecked(rodSpeed == 3);

    ui->buttonCenter->setChecked(centerOnly == 0);

    float thermal[25];
    m_api->GETTHR(thermal);

    for(int i = 0; i < 25; i++)
    {
        m_thermal[i]->setText(QString::number(thermal[i]*100, 'f', 1)+"%");
    }

    for(int x = 0; x < 5; x++)
    {
        for(int y = 0; y < 5; y++)
        {
            float rod = m_api->GETROD(x, y);
            int id = x + 5*y;

            m_positions[id]->setText(QString::number(rod*100, 'f', 1)+"%");
        }
    }
}

void ReactorControlRods::rodInc(int x, int y)
{

}

void ReactorControlRods::rodDec(int x, int y)
{

}

void ReactorControlRods::pullRods()
{
    m_api->SETROD(-1, 0, 0, 1);
}

void ReactorControlRods::holdRods()
{
    m_api->SETROD(0, 0, 0, 1);
}

void ReactorControlRods::insertRods()
{
    m_api->SETROD(1, 0, 0, 1);
}

void ReactorControlRods::speedSlow()
{
    m_api->SETROD(1, 0, 0, 3);
}

void ReactorControlRods::speedMedium()
{
    m_api->SETROD(2, 0, 0, 3);
}

void ReactorControlRods::speedFast()
{
    m_api->SETROD(3, 0, 0, 3);
}

void ReactorControlRods::centerOnly()
{
    int centerOnly = m_api->GETMOV(2);

    m_api->SETROD(!centerOnly, 0, 0, 4);
}
