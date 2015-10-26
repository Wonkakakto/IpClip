#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this -> setTrayIconActions();
    this -> showTrayIcon();

    setWindowFlags(Qt::Window
       //| Qt::WindowMinimizeButtonHint
       //| Qt::WindowMaximizeButtonHint
       | Qt::CustomizeWindowHint
       | Qt::WindowCloseButtonHint);


    QString pathForStoreSettings = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)) + QDir::separator();

    /*#ifdef Q_OS_WIN32
        pathForStoreSettings = qgetenv("AppData") + QDir::separator();
    #else
        pathForStoreSettings = pathForStoreSettings + ".config" + QDir::separator();
    #endif*/

    QDir dir = QDir::root();
    dir.mkpath( pathForStoreSettings );

    this->settingsApp = new QSettings( pathForStoreSettings +
                                 QCoreApplication::applicationName() + ".ini",
                                 QSettings::IniFormat);
    readSettings();

    ipList = new QStringList;
    readIpList();

    /*QString str;
    str.setNum(ipList->count());
    QMessageBox::information(this, "TrayIcon", "summ= " + str);*/
}

MainWindow::~MainWindow()
{
    delete ipList;
    delete this->settingsApp;
    delete ui;
}

void MainWindow::showTrayIcon()
{
    // Создаём экземпляр класса и задаём его свойства...
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(":/images/images/icon.png");
    trayIcon -> setIcon(trayImage);
    trayIcon -> setContextMenu(trayIconMenu);

    // Подключаем обработчик клика по иконке...
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    // Выводим значок...
    trayIcon -> show();
}

void MainWindow::trayActionExecute()
{
    if (this->isHidden())
    {
        showNormal();
    }
    else
    {
        this->setHidden(true);
    }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this -> trayActionExecute();
            break;
        default:
            break;
    }
}

void MainWindow::setTrayIconActions()
{
    // Setting actions...
    minimizeAction = new QAction("Свернуть", this);
    restoreAction = new QAction("Восстановить", this);
    quitAction = new QAction("Выход", this);
    showWindowSetSettingsAction = new QAction("Настройки", this);

    // Connecting actions to slots...
    connect (minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect (restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect (quitAction, SIGNAL(triggered()), this, SLOT(quitFromApp()));
    connect (showWindowSetSettingsAction, SIGNAL(triggered()), this, SLOT(showWindowSetSettings()));

    // Setting system tray's icon menu...
    trayIconMenu = new QMenu(this);
    trayIconMenu -> addAction (minimizeAction);
    trayIconMenu -> addAction (restoreAction);
    trayIconMenu -> addAction (showWindowSetSettingsAction);
    trayIconMenu -> addAction (quitAction);
}

void MainWindow::quitFromApp()
{
    writeSettings();
    qApp->quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this -> hide();
}

void MainWindow::showEvent(QShowEvent * event)
{
    ui->cbFind->setFocus();
}

//Сворачиваем в трей, вместо панели задач:
/*void MainWindow::changeEvent(QEvent *event)
{
        QMainWindow::changeEvent(event);
    if (event -> type() == QEvent::WindowStateChange)
    {
        if (isMinimized())
        {
            this -> hide();
        }
    }

}*/

void MainWindow::on_pbClose_clicked()
{
    this -> close();
    //qApp->quit();
}

void MainWindow::showWindowSetSettings()
{

    SetSettingsDialog = new SetSettings;
    connect(this, SIGNAL(sendAppSettings(QSettings*)), SetSettingsDialog, SLOT(recieveAppSettings(QSettings*)));

    emit sendAppSettings(this->settingsApp);
    SetSettingsDialog->exec();
    /*if (SetSettingsDialog->exec() == QDialog::Accepted)
    {
      //исправить: здесь нужно перечитать новые настройки
    }*/
    //SetSettingsDialog->show();

        //QMessageBox::information(this, "TrayIcon", "Сохраняем и применяем новые настройки приложения!");
    delete SetSettingsDialog;

}

void MainWindow::writeSettings()
{
    settingsApp->beginGroup("MainWindow");
    settingsApp->setValue("size", size());
    settingsApp->setValue("pos", pos());
    settingsApp->endGroup();
}

void MainWindow::readSettings()
{
    settingsApp->beginGroup("MainWindow");
    if(settingsApp->value("saveposition",true).toBool())
    {
        resize(settingsApp->value("size",QSize(this->width(),this->height())).toSize());
        move(settingsApp->value("pos",QPoint(200,200)).toPoint());
    }
    settingsApp->endGroup();
}

void MainWindow::readIpList()
{
    QFile inFile;
    ipList->clear();
    int size = settingsApp->beginReadArray("Files");
    for(int i=0;i<size;i++)
    {
        settingsApp->setArrayIndex(i);
        inFile.setFileName(settingsApp->value("File").toString());
        if (inFile.open(QIODevice::ReadOnly))
        {
            while(!inFile.atEnd())
            {
                ipList->append(inFile.readLine());
            }
        }
        inFile.close();
    }
    settingsApp->endArray();
}

void MainWindow::on_cbFind_currentTextChanged(const QString &arg1)
{
    ui->cbResult->clear();
    if ( arg1.count() > 2 )
    {
        int i = ipList->indexOf(QRegExp(".*" + QRegExp::escape(arg1) + ".*"));
        while(i!=-1)
        {
            ui->cbResult->addItem(ipList->at(i));
            i++;
            i = ipList->indexOf(QRegExp(".*" + QRegExp::escape(arg1) + ".*"),i);
        }
    }
}
