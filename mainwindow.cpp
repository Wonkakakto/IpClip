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

    QDir dir = QDir::root();
    dir.mkpath( pathForStoreSettings );

    this->settingsApp = new QSettings( pathForStoreSettings +
                                 QCoreApplication::applicationName() + ".ini",
                                 QSettings::IniFormat);
    readMainWindowPositionAndSize();

    ipList = new QStringList;
    this -> readIpList();

    CommandsName = new QStringList;
    CommandsPath = new QStringList;
    CommandsParam = new QStringList;
    this -> readCommandsList();

    commandMainMenu = new QMenu(this);
    this -> setMainMenuActions();

}

MainWindow::~MainWindow()
{
    delete ipList;
    //delete DefaultCommand;
    delete CommandsName;
    delete CommandsPath;
    delete CommandsParam;
    delete commandMainMenu;
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

    if (SetSettingsDialog->exec() == QDialog::Accepted)
    {
        this -> readIpList();
        this -> readCommandsList();
        this -> setMainMenuActions();
    }
    delete SetSettingsDialog;

}

void MainWindow::writeSettings()
{
    settingsApp->beginGroup("MainWindow");
    settingsApp->setValue("Size", size());
    settingsApp->setValue("Pos", pos());
    settingsApp->endGroup();
}

void MainWindow::readMainWindowPositionAndSize()
{
    settingsApp->beginGroup("MainWindow");
    if(settingsApp->value("SavePosition",true).toBool())
    {
        resize(settingsApp->value("Size",QSize(this->width(),this->height())).toSize());
        move(settingsApp->value("Pos",QPoint(200,200)).toPoint());
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

void MainWindow::readCommandsList()
{
    settingsApp->beginGroup("MainWindow");
    DefaultCommand[0] = settingsApp->value("DefaultCommand","Ping").toString();
    settingsApp->endGroup();
    DefaultCommand[1] = "ping";
    DefaultCommand[2] = "%s";

    int size = settingsApp->beginReadArray("Commands");
    CommandsName->clear();
    CommandsPath->clear();
    CommandsParam->clear();
    for(int j=0;j<size;++j)
    {
        settingsApp->setArrayIndex(j);
        CommandsName->append(settingsApp->value("CommandName").toString());
        CommandsPath->append(settingsApp->value("CommandPath").toString());
        CommandsParam->append(settingsApp->value("CommandParams").toString());
        if(DefaultCommand[0]==settingsApp->value("CommandName").toString())
        {
            DefaultCommand[1] = settingsApp->value("CommandPath").toString();
            DefaultCommand[2] = settingsApp->value("CommandParams").toString();
        }
    }
    settingsApp->endArray();
}

void MainWindow::setMainMenuActions()
{
    ui->bpDoIt->setText( DefaultCommand[0] );

    commandMainMenu->clear();

    int size = CommandsName->count();
    for(int i=0;i<size;++i)
    {
        commandMainMenu->addAction(CommandsName->at(i));
    }

}

void MainWindow::on_bpDoIt_customContextMenuRequested(const QPoint &pos)
{
    QAction* selectedItem = commandMainMenu->exec(QCursor::pos());
        if(selectedItem)
            QMessageBox::information(this, "TrayIcon", selectedItem->text());
   // if(selectedItem)
     //   on_bpDoIt_Menu_clicked(selectedItem);
}

void MainWindow::on_bpDoIt_clicked()
{
    QMessageBox::information(this, "TrayIcon", DefaultCommand[0]);
}

/*void MainWindow::on_bpDoIt_Menu_clicked(QAction *event)
{
    QMessageBox::information(this, "TrayIcon", event->text());
}*/
