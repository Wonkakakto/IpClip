#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("HSoft");
    QCoreApplication::setApplicationName("IpClip");
    MainWindow w;
    w.show();

    return a.exec();
}
