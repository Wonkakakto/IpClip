#include "mainwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setApplicationVersion(APP_VERSION);
    qApp->setQuitOnLastWindowClosed(false);
    //a.setApplicationVersion(APP_VERSION);
    //QCoreApplication::setOrganizationName("HSoft");
    //QCoreApplication::setApplicationName("IpClip");
    //QCoreApplication::setApplicationName(QMAKE_TARGET_PRODUCT);
    MainWindow w;
    if( argc == 1 ) w.show();

    return a.exec();
}
