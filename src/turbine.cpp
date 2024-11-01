#include "turbine.h"
#include "ui_turbine.h"

Turbine::Turbine(QWidget *parent, Chrnob32API *api) :
    QDialog(parent),
    ui(new Ui::Turbine),
    m_api(api)
{
    ui->setupUi(this);
}

Turbine::~Turbine()
{
    delete ui;
}

void Turbine::refresh()
{

}
