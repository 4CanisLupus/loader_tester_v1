#include "mainwindow.h"
#include "logger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initLogging();

    MainWindow w;
    w.show();
    return QApplication::exec();
}
