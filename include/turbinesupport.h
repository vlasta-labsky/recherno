#ifndef TURBINESUPPORT_H
#define TURBINESUPPORT_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class TurbineSupport;
}

class TurbineSupport : public QDialog
{
    Q_OBJECT

public:
    explicit TurbineSupport(QWidget *parent, Chrnob32API *api);
    ~TurbineSupport();

    void refresh();

protected slots:
    void lubeStart();
    void lubeStop();

    void hydraulicStart();
    void hydraulicStop();

    void steamSealStart();
    void steamSealStop();

    void steamDrainOpen();
    void steamDrainClose();

private:
    Ui::TurbineSupport *ui;

    Chrnob32API *m_api;
};

#endif // TURBINESUPPORT_H
