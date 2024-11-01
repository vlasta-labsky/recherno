#ifndef FEEDWATER_H
#define FEEDWATER_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class FeedWater;
}

class FeedWater : public QDialog
{
    Q_OBJECT

public:
    explicit FeedWater(QWidget *parent, Chrnob32API *api);
    ~FeedWater();

    void refresh();

protected slots:
    void toggleFeedPump1();
    void toggleFeedPump2();
    void toggleFeedPump3();

    void feedInlet1Open();
    void feedInlet2Open();
    void feedInlet3Open();
    void feedOutlet1Open();
    void feedOutlet2Open();
    void feedOutlet3Open();

    void feedInlet1Close();
    void feedInlet2Close();
    void feedInlet3Close();
    void feedOutlet1Close();
    void feedOutlet2Close();
    void feedOutlet3Close();

    void setManualValve();
    void setAutoValve();
    void toggle3Element();
    void setStartupValve();
    void setMainValve();

    void drainAuto();
    void drainManual();

    void drainInc();
    void drainDec();

private:
    Ui::FeedWater *ui;

    Chrnob32API *m_api;
};

#endif // FEEDWATER_H
