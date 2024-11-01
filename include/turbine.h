#ifndef TURBINE_H
#define TURBINE_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class Turbine;
}

class Turbine : public QDialog
{
    Q_OBJECT

public:
    explicit Turbine(QWidget *parent, Chrnob32API *api);
    ~Turbine();

    void refresh();

private:
    Ui::Turbine *ui;

    Chrnob32API *m_api;
};

#endif // TURBINE_H
