#ifndef SYSTEMSUMMARY_H
#define SYSTEMSUMMARY_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class SystemSummary;
}

class SystemSummary : public QDialog
{
    Q_OBJECT

public:
    explicit SystemSummary(QWidget *parent, Chrnob32API *api);
    ~SystemSummary();

    void refresh();

private:
    Ui::SystemSummary *ui;

    Chrnob32API *m_api;
};

#endif // SYSTEMSUMMARY_H
