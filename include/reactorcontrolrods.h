#ifndef REACTORCONTROLRODS_H
#define REACTORCONTROLRODS_H

#include <QDialog>

class Chrnob32API;

class QLineEdit;

namespace Ui {
class ReactorControlRods;
}

class ReactorControlRods : public QDialog
{
    Q_OBJECT

public:
    explicit ReactorControlRods(QWidget *parent, Chrnob32API *api);
    ~ReactorControlRods();

    void refresh();

protected:
    void rodInc(int x, int y);
    void rodDec(int x, int y);

protected slots:
    void pullRods();
    void holdRods();
    void insertRods();

    void speedSlow();
    void speedMedium();
    void speedFast();

    void centerOnly();

private:
    Ui::ReactorControlRods *ui;

    QLineEdit *m_positions[25];
    QLineEdit *m_temps[25];
    QLineEdit *m_thermal[25];

    Chrnob32API *m_api;
};

#endif // REACTORCONTROLRODS_H
