#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this -> setTrayIcon();

    //| Qt::WindowMinimizeButtonHint
    //| Qt::WindowMaximizeButtonHint
    setWindowFlags(Qt::Window |
                   Qt::CustomizeWindowHint |
                   Qt::WindowCloseButtonHint);

    QString pathForStoreSettings = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)) +
                                                            QDir::separator();

    QDir dir = QDir::root();
    dir.mkpath(pathForStoreSettings);

    this->settingsApp = new QSettings(pathForStoreSettings +
                                      QCoreApplication::applicationName() + ".ini",
                                      QSettings::IniFormat);
    readMainWindowPositionAndSize();

    this -> readIpList();
    this -> readCommandsList();

    name2bufferAction.setText(tr("Имя в буфер"));
    connect(&name2bufferAction, SIGNAL(triggered()), this, SLOT(copyName2Buffer()));
    ip2bufMenu.addAction(&name2bufferAction);

    closeAppAction.setText(tr("Выход из приложения"));
    connect(&closeAppAction, SIGNAL(triggered()), this, SLOT(quitFromApp()));
    closeButtonMenu.addAction(&closeAppAction);

}

MainWindow::~MainWindow()
{

    delete settingsApp;
    delete ui;

}

void MainWindow::setTrayIcon()
{

    // Setting actions...
    minimizeAction.setText(tr("Свернуть"));
    restoreAction.setText(tr("Восстановить"));
    showWindowSetSettingsAction.setText(tr("Настройки"));
    makeUpdate.setText(tr("Обновить"));
    quitAction.setText(tr("Выход"));

    // Connecting actions to slots...
    connect (&minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect (&restoreAction, SIGNAL(triggered()), this, SLOT(restoreWindow()));
    connect (&showWindowSetSettingsAction, SIGNAL(triggered()), this, SLOT(showWindowSetSettings()));
    connect (&makeUpdate, SIGNAL(triggered()), this, SLOT(readIpList()));
    connect (&quitAction, SIGNAL(triggered()), this, SLOT(quitFromApp()));

    // Setting system tray's icon menu...
    trayIconMenu.addAction (&minimizeAction);
    trayIconMenu.addAction (&restoreAction);
    trayIconMenu.addAction (&showWindowSetSettingsAction);
    trayIconMenu.addAction (&makeUpdate);
    trayIconMenu.addAction (&quitAction);

    connect(&trayIconMenu, SIGNAL(aboutToShow()), this, SLOT(onShowTrayMenu()));
    connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(onTrayIconActivate(QSystemTrayIcon::ActivationReason)));

    trayIcon.setIcon(QIcon(":/images/images/icon.ico").pixmap(16, 16) );
    trayIcon.setContextMenu(&trayIconMenu);
    trayIcon.show();

}

void MainWindow::onShowTrayMenu()
{

    this -> minimizeAction.setEnabled(!this -> isHidden());
    this -> restoreAction.setEnabled(!this -> isActiveWindow());

}

void MainWindow::onTrayIconActivate(QSystemTrayIcon::ActivationReason r)
{

    if((r == QSystemTrayIcon::DoubleClick) ||
        (r == QSystemTrayIcon::Trigger) ||
        (r ==  QSystemTrayIcon::MiddleClick))
    {

            restoreWindow();

    }

}

void MainWindow::restoreWindow()
{

    if (this -> isHidden())
    {

        this -> showNormal();

    }

    if (!this -> isActiveWindow())
    {

        this -> activateWindow();

    }

}

void MainWindow::quitFromApp()
{

    writeSettings();
    qApp -> quit();

}

void MainWindow::closeEvent(QCloseEvent * event)
{

    event -> ignore();
    this -> hide();

}

void MainWindow::showEvent(QShowEvent * event)
{

    ui -> cbFind -> setFocus();

}

void MainWindow::on_pbClose_clicked()
{

    this -> close();
    //qApp->quit();

}

void MainWindow::showWindowSetSettings()
{

    SetSettingsDialog = new SetSettings;

    connect(this, SIGNAL(sendAppSettings(QSettings *)), SetSettingsDialog,
             SLOT(recieveAppSettings(QSettings *)));

    emit sendAppSettings(this -> settingsApp);

    if (SetSettingsDialog->exec() == QDialog::Accepted)
    {

        this -> readIpList();
        this -> readCommandsList();

    }

    delete SetSettingsDialog;

}

void MainWindow::writeSettings()
{

    this -> settingsApp -> beginGroup("MainWindow");
    this -> settingsApp -> setValue("Size", size());
    this -> settingsApp -> setValue("Pos", pos());
    this -> settingsApp -> endGroup();
    settingsApp -> sync();

}

void MainWindow::readMainWindowPositionAndSize()
{

    settingsApp -> beginGroup("MainWindow");

    if (settingsApp -> value("SavePosition", true).toBool())
    {

        resize(settingsApp -> value("Size", QSize(this -> width(), this -> height())).toSize());
        move(settingsApp -> value("Pos", QPoint(200, 200)).toPoint());

    }

    settingsApp -> endGroup();

}

void MainWindow::readIpList()
{

    QFile inFile;
    QTextStream inTextStream;
    QString sTmp;
    int iSharp;

    ipList.clear();

    int size = settingsApp -> beginReadArray("Files");

    for (int i = 0; i < size; i++)
    {

        settingsApp -> setArrayIndex(i);
        inFile.setFileName(settingsApp -> value("File").toString());

        if (inFile.open(QIODevice::ReadOnly))
        {

            inTextStream.setDevice(&inFile);

            while (!inTextStream.atEnd())
            {

                sTmp = inTextStream.readLine();
                iSharp = sTmp.indexOf(QString("#"));
                ipList.append(sTmp.left(iSharp).trimmed() + " [" +
                               sTmp.right(sTmp.length() - iSharp - 1).trimmed() + "]");

            }

        }

        inFile.close();

    }

    settingsApp -> endArray();

}

void MainWindow::on_cbFind_currentTextChanged(const QString &arg1)
{

    ui -> cbResult -> clear();

    if (arg1.count() > 2)
    {

        int i = ipList.indexOf(QRegExp(".*" + QRegExp::escape(arg1) + ".*", Qt::CaseInsensitive));

        while (i != -1)
        {

            ui -> cbResult -> addItem(ipList.at(i));
            i++;
            i = ipList.indexOf(QRegExp(".*" + QRegExp::escape(arg1) + ".*", Qt::CaseInsensitive), i);

        }

    }

}

void MainWindow::readCommandsList()
{

    settingsApp -> beginGroup("MainWindow");
    defaultCommand[0] = settingsApp -> value("DefaultCommand", "Ping").toString();
    settingsApp -> endGroup();

    defaultCommand[1] = "ping %s";
    commandMainMenu.clear();

    QAction* action;
    int size = settingsApp -> beginReadArray("Commands");

    for (int j = 0; j < size; ++j)
    {

        settingsApp -> setArrayIndex(j);

        if (defaultCommand[0] == settingsApp -> value("CommandName").toString())
        {

            defaultCommand[1] = settingsApp -> value("CommandPath").toString() + " " +
                                settingsApp -> value("CommandParams").toString();

        }

        action = new QAction(settingsApp -> value("CommandName").toString());
        action -> setData(QString(settingsApp -> value("CommandPath").toString() + " " +
                                    settingsApp -> value("CommandParams").toString()));
        commandMainMenu.addAction(action);

    }

    settingsApp -> endArray();

}

void MainWindow::on_bpDoIt_customContextMenuRequested(const QPoint &pos)
{

    QAction* selectedItem = commandMainMenu.exec(QCursor::pos());

    if (selectedItem)
    {

        makeDoIt(selectedItem -> data().toString());

    }

}

void MainWindow::makeDoIt(QString cmdName)
{

    QThread* thread = new QThread;
    RunExternalCommand* oRun = new RunExternalCommand(cmdName.replace("%s", getIPfromStr()));

    oRun->moveToThread(thread);

    connect(thread, SIGNAL(started()), oRun, SLOT(execute()));
    connect(oRun, SIGNAL(finished()), thread, SLOT(quit()));
    connect(oRun, SIGNAL(finished()), oRun, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread -> start();

}

void MainWindow::on_bpDoIt_clicked()
{

    makeDoIt(defaultCommand[1]);

}

QString MainWindow::getIPfromStr()
{

    QString strTmp = ui -> cbResult -> currentText();

    if (strTmp.length() > 0)
    {

        int iIndexSharp = strTmp.indexOf(QString("["));

        if(iIndexSharp >= 0)
        {

            strTmp.truncate(iIndexSharp);

        }

        strTmp = strTmp.trimmed();

    } else {

        strTmp = "";

    }

    return strTmp;

}

void MainWindow::on_pbIP2Buf_clicked()
{

    QApplication::clipboard() -> setText(getIPfromStr());

}

void MainWindow::copyName2Buffer()
{

    QString strTmp = ui -> cbResult -> currentText();

    if (strTmp.length() > 0)
    {

        int iIndexSharp = strTmp.indexOf(QString("["));

        if (iIndexSharp == -1)
        {

            strTmp = "";

        } else {

            strTmp = strTmp.right(strTmp.length() - iIndexSharp - 1);

        }

        QApplication::clipboard() -> setText(strTmp.left(strTmp.indexOf(QString("]"))));
    }

}

void MainWindow::on_pbIP2Buf_customContextMenuRequested(const QPoint &pos)
{

    ip2bufMenu.exec(QCursor::pos());

}

void MainWindow::on_pbClose_customContextMenuRequested(const QPoint &pos)
{

    closeButtonMenu.exec(QCursor::pos());

}
