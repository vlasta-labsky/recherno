#include <QApplication>

#include "virtualmachine.h"
#include "math.h"

#include "mainwindow.h"

#include "impl.h"
#include "api.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
