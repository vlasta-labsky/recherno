#ifndef OFFLINECOOLING_H
#define OFFLINECOOLING_H

#include <QDialog>

class Chrnob32API;

namespace Ui {
class OfflineCooling;
}

class OfflineCooling : public QDialog
{
    Q_OBJECT

public:
    explicit OfflineCooling(QWidget *parent, Chrnob32API *api);
    ~OfflineCooling();

    void refresh();

protected slots:
    void loop1Inc();
    void loop1Dec();
    void loop2Inc();
    void loop2Dec();

    void pump1Start();
    void pump1Stop();
    void pump1InletOpen();
    void pump1InletClose();
    void pump1OutletOpen();
    void pump1OutletClose();

    void pump2Start();
    void pump2Stop();
    void pump2InletOpen();
    void pump2InletClose();
    void pump2OutletOpen();
    void pump2OutletClose();

private:
    Ui::OfflineCooling *ui;

    Chrnob32API *m_api;
};

#endif // OFFLINECOOLING_H
