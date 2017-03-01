#include "setsettings.h"
#include "ui_setsettings.h"

#include <QTextCodec>
#include <QClipboard>

SetSettings::SetSettings(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::SetSettings)
{

    ui -> setupUi(this);
    editCommandWindow = new editCommand(this);

    connect(this, SIGNAL(sendCommandSettings(QString, QString, QString, bool)),
            editCommandWindow, SLOT(receiveCommandSettings(QString, QString, QString, bool)));

    ui -> tvCommandsList -> setColumnCount(4);
    ui -> tvCommandsList -> setHorizontalHeaderItem(0, new QTableWidgetItem(""));
    ui -> tvCommandsList -> setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Команда")));
    ui -> tvCommandsList -> setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Имя исполняемого файла")));
    ui -> tvCommandsList -> setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Параметры")));
    ui -> tvCommandsList -> horizontalHeader() -> setSectionResizeMode(QHeaderView::ResizeToContents);
    ui -> tvCommandsList -> horizontalHeader() -> setSectionResizeMode(0, QHeaderView::ResizeToContents);//Fixed
    //ui->tvCommandsList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui -> tvCommandsList -> horizontalHeader() -> setSectionResizeMode(2, QHeaderView::Interactive);
    //ui->tvCommandsList->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);

    ui -> tvCommandsList -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui -> tvCommandsList -> setSelectionBehavior(QAbstractItemView::SelectRows);

    #ifdef Q_OS_WIN32
        ui -> pbImportSettings -> setEnabled(true);
        ui -> pbStartup -> setEnabled(true);
    #else
        ui -> pbImportSettings -> setEnabled(false);
        ui -> pbStartup -> setEnabled(false);
    #endif

}

SetSettings::~SetSettings()
{

    delete this -> editCommandWindow;
    delete ui;

}

void SetSettings::on_pbAddFile_clicked()
{

    ui -> lvFileList -> addItems(QFileDialog::getOpenFileNames(this, tr("Выбрать файл:"), ".",
                                                               tr("Файл со списком адресов *.txt (*.*)")));

}

void SetSettings::on_pbDeleteFile_clicked()
{

    delete ui -> lvFileList -> currentItem();

}

void SetSettings::on_pbSetDefaults_clicked()
{

    cleanSettings();
    receiveCommandParam("Ping", "ping", "%s", true);
    setDefaultCommandInCmdList(0);
    //QTableWidgetItem* item;
    //ui->tvCommandsList->setItem(0,0,new QTableWidgetItem(QPixmap(":/images/images/icon.png"),""));
    /*QString s;
    s = (QString)(ui->keySequenceEdit->keySequence().toString());
    QMessageBox::information(this, "TrayIcon", s);
    QKeySequence()*/
    //ui->keySequenceEdit->keySequence();

}

void SetSettings::on_buttonBox_accepted()
{

    writeSettings();

}

void SetSettings::writeSettings()
{

    settingsApp -> remove("Files");
    settingsApp -> beginWriteArray("Files");

    for (int i = 0; i < ui -> lvFileList -> count(); ++i)
    {

        settingsApp -> setArrayIndex(i);
        settingsApp -> setValue("File", ui -> lvFileList -> item(i) -> text());

    }

    settingsApp -> endArray();

    settingsApp -> beginGroup("MainWindow");
    settingsApp -> setValue("SavePosition", ui -> cbSaveMainWindowPosition -> checkState());
    settingsApp -> endGroup();

    QString defaultCommand = "Ping";

    settingsApp -> remove("Commands");
    settingsApp -> beginWriteArray("Commands");

    for (int j = 0; j < ui -> tvCommandsList -> rowCount(); ++j)
    {

        settingsApp -> setArrayIndex(j);
        settingsApp -> setValue("CommandName", ui -> tvCommandsList -> item(j, 1) -> text());
        settingsApp -> setValue("CommandPath", ui -> tvCommandsList -> item(j, 2) -> text());
        settingsApp -> setValue("CommandParams", ui -> tvCommandsList -> item(j, 3) -> text());

        if (ui -> tvCommandsList -> item(j, 0) != 0)
        {

            defaultCommand = ui -> tvCommandsList -> item(j, 1) -> text();

        }

    }

    settingsApp -> endArray();
    settingsApp -> beginGroup("MainWindow");
    settingsApp -> setValue("DefaultCommand", defaultCommand);
    settingsApp -> endGroup();

}

void SetSettings::readSettings()
{
    QString defaultCommand = "Ping";

    settingsApp -> beginGroup("MainWindow");
    defaultCommand = settingsApp -> value("DefaultCommand", "Ping").toString();
    settingsApp -> endGroup();

    int size = settingsApp -> beginReadArray("Files");

    for (int i = 0; i < size; ++i)
    {

        settingsApp -> setArrayIndex(i);
        ui -> lvFileList -> addItem(settingsApp -> value("File").toString());

    }

    settingsApp -> endArray();

    settingsApp -> beginGroup("MainWindow");
    ui -> cbSaveMainWindowPosition -> setChecked(settingsApp -> value("SavePosition", true).toBool());
    settingsApp -> endGroup();

    size = settingsApp -> beginReadArray("Commands");
    ui -> tvCommandsList -> setRowCount(size);

    for (int j = 0; j < size; ++j)
    {

        settingsApp -> setArrayIndex(j);
        ui -> tvCommandsList -> setItem(j, 1,
                                        new QTableWidgetItem(settingsApp -> value("CommandName").toString()));
        ui -> tvCommandsList -> setItem(j, 2,
                                        new QTableWidgetItem(settingsApp -> value("CommandPath").toString()));
        ui -> tvCommandsList -> setItem(j, 3,
                                        new QTableWidgetItem(settingsApp -> value("CommandParams").toString()));

        if (defaultCommand == settingsApp -> value("CommandName").toString())
        {

            //ui->tvCommandsList->setItem(j,0,new QTableWidgetItem(QPixmap(":/images/images/icon.png"),""));
            setDefaultCommandInCmdList(j);

        }

    }

    settingsApp -> endArray();

}

void SetSettings::recieveAppSettings(QSettings * sp)
{

    this -> settingsApp = sp;
    readSettings();
    updateAboutText();

}

void SetSettings::on_pbEditCommand_clicked()
{

    int row = ui -> tvCommandsList -> currentRow();

    if (row != -1)
    {

        emit sendCommandSettings(ui -> tvCommandsList -> item(row, 1) -> text(),
                                 ui -> tvCommandsList -> item(row, 2) -> text(),
                                 ui -> tvCommandsList -> item(row, 3) -> text(), false);
        editCommandWindow -> exec();

    }

}

void SetSettings::on_pbAddCommand_clicked()
{

    emit sendCommandSettings("", "", "", true);
    editCommandWindow -> exec();

}

void SetSettings::receiveCommandParam(QString sNameCmd, QString sFileNameExec, QString sParams, bool bNewCommand)
{

    int row = 0;

    if (bNewCommand)
    {

        row = ui -> tvCommandsList -> rowCount();
        ui -> tvCommandsList -> setRowCount(row + 1);
        ui -> tvCommandsList -> setItem(row, 1, new QTableWidgetItem(sNameCmd));
        ui -> tvCommandsList -> setItem(row, 2, new QTableWidgetItem(sFileNameExec));
        ui -> tvCommandsList -> setItem(row, 3, new QTableWidgetItem(sParams));

    } else {

        row = ui -> tvCommandsList -> currentRow();
        ui -> tvCommandsList -> item(row, 1) -> setText(sNameCmd);
        ui -> tvCommandsList -> item(row, 2) -> setText(sFileNameExec);
        ui -> tvCommandsList -> item(row, 3) -> setText(sParams);

    }

}

void SetSettings::on_pbDeleteCommand_clicked()
{

    int row = ui -> tvCommandsList -> currentRow();

    if ( row != -1 )
    {

        ui -> tvCommandsList -> removeRow(row);

    }

}

void SetSettings::on_bpDefaultCommand_clicked()
{

    int row = ui -> tvCommandsList -> currentRow();

    setDefaultCommandInCmdList(row);

}

void SetSettings::setDefaultCommandInCmdList(int row)
{

    if ( row != -1 )
    {

        QTableWidgetItem* item;

        item = ui -> tvCommandsList -> item(row, 0);

        if (item == 0)
        {

            ui -> tvCommandsList -> setItem(row, 0, new QTableWidgetItem(QPixmap(":/images/images/icon.ico"), ""));

        } else {

            item -> setIcon(QPixmap(":/images/images/icon.ico"));

        }

        for (int i = 0; i < ui -> tvCommandsList -> rowCount(); ++i)
        {

            if ((i != row) && (ui -> tvCommandsList -> item(i, 0) !=0))
            {

                delete ui -> tvCommandsList -> item(i, 0);

            }

        }

        ui -> bpDefaultCommand -> setEnabled(false);

    }

}

void SetSettings::on_pbUpCommand_clicked()
{

    int rowcount = ui -> tvCommandsList -> rowCount();
    int row = ui -> tvCommandsList -> currentRow();

    //if(row == -1 || row == 0 || rowcount < 2)
    if ((row > 0) && (rowcount > 1))
    {

        QTableWidgetItem* item;

        if ((ui -> tvCommandsList -> item(row, 0) != 0) || (ui -> tvCommandsList -> item(row - 1, 0) != 0))
        {

            if (ui -> tvCommandsList -> item(row, 0) != 0)
            {

                ui -> tvCommandsList -> setItem(row - 1, 0, ui -> tvCommandsList -> takeItem(row, 0));
                delete ui -> tvCommandsList -> item(row, 0);

            } else {

                ui -> tvCommandsList -> setItem(row, 0, ui -> tvCommandsList -> takeItem(row - 1, 0));
                delete ui -> tvCommandsList -> item(row - 1, 0);

            }

        }

        for (int i = 1; i < 4; ++i)
        {

            item = ui -> tvCommandsList -> takeItem(row, i);
            ui -> tvCommandsList -> setItem(row, i, ui -> tvCommandsList -> takeItem(row - 1, i));
            ui -> tvCommandsList -> setItem(row - 1, i, item);

        }

        ui -> tvCommandsList -> selectRow(row - 1);

    }

}

void SetSettings::on_pbDownCommand_clicked()
{

    int rowcount = ui -> tvCommandsList -> rowCount();
    int row = ui -> tvCommandsList -> currentRow();
    // row != -1 and row != rowcount - 1 and rowcount > 1

    if ((row != -1) && (rowcount > 1) && (row != rowcount - 1))
    {

        QTableWidgetItem * item;

        if ((ui -> tvCommandsList -> item(row, 0) != 0) || (ui -> tvCommandsList -> item(row + 1, 0) != 0))
        {

            if (ui -> tvCommandsList -> item(row, 0) != 0)
            {

                ui -> tvCommandsList -> setItem(row + 1, 0, ui -> tvCommandsList -> takeItem(row, 0));
                delete ui -> tvCommandsList -> item(row, 0);

            } else {

                ui -> tvCommandsList -> setItem(row, 0, ui -> tvCommandsList -> takeItem(row + 1, 0));
                delete ui -> tvCommandsList -> item(row + 1, 0);

            }

        }

        for (int i = 1; i < 4; ++i)
        {

            item = ui -> tvCommandsList -> takeItem(row, i);
            ui -> tvCommandsList -> setItem(row, i, ui -> tvCommandsList -> takeItem(row + 1, i));
            ui -> tvCommandsList -> setItem(row + 1, i, item);

        }

        ui -> tvCommandsList -> selectRow(row + 1);

    }

}

void SetSettings::on_lvFileList_itemSelectionChanged()
{

    ui -> pbDeleteFile -> setEnabled(ui -> lvFileList -> selectedItems().count() > 0);
    ui -> pbUpFile -> setEnabled((ui -> lvFileList -> selectedItems().count() > 0) &&
                                 (ui -> lvFileList -> currentRow() > 0));
    ui -> pbDownFile -> setEnabled((ui -> lvFileList -> selectedItems().count() > 0) &&
                                   (ui -> lvFileList -> currentRow() < ui -> lvFileList -> count() - 1));

}

void SetSettings::on_tvCommandsList_itemSelectionChanged()
{

    ui -> pbDeleteCommand -> setEnabled(ui -> tvCommandsList -> selectedItems().count() > 0);
    ui -> pbEditCommand -> setEnabled(ui -> tvCommandsList -> selectedItems().count() > 0);
    ui -> pbDeleteCommand -> setEnabled(ui -> tvCommandsList -> selectedItems().count() > 0);
    ui -> bpDefaultCommand -> setEnabled((ui -> tvCommandsList -> selectedItems().count() > 0) &&
                                         (ui -> tvCommandsList -> item(ui -> tvCommandsList -> currentRow(), 0) == 0));
    ui -> pbUpCommand -> setEnabled((ui -> tvCommandsList -> selectedItems().count() > 0) &&
                                    (ui -> tvCommandsList -> currentRow() > 0));
    ui -> pbDownCommand -> setEnabled((ui -> tvCommandsList -> selectedItems().count() > 0) &&
                                      (ui -> tvCommandsList -> currentRow() < ui -> tvCommandsList -> rowCount() - 1));

}

void SetSettings::on_pbUpFile_clicked()
{

    int currentRow = ui -> lvFileList -> currentRow();

    if (currentRow > 0)
    {

        ui -> lvFileList -> insertItem(currentRow - 1, ui -> lvFileList -> takeItem(currentRow));
        ui -> lvFileList -> setCurrentRow(currentRow - 1);

    }

}

void SetSettings::on_pbDownFile_clicked()
{

    int currentRow = ui -> lvFileList -> currentRow();

    if (currentRow < (ui -> lvFileList -> count() - 1))
    {

        ui -> lvFileList -> insertItem(currentRow + 1, ui -> lvFileList -> takeItem(currentRow));
        ui -> lvFileList -> setCurrentRow(currentRow + 1);

    }

}

void SetSettings::cleanSettings()
{

    ui -> lvFileList -> clear();
    //ui->tvCommandsList->clear();
    //ui->tvCommandsList->clearContents();

    while (ui -> tvCommandsList -> rowCount() > 0)
    {

        ui -> tvCommandsList -> removeRow(0);

    }

}

void SetSettings::on_pbImportSettings_clicked()
{
    #ifdef Q_OS_WIN32

    cleanSettings();

    QString importFilePath = QFileDialog::getOpenFileName(this,
                                                           tr("Импорт настроек ClipIp:"),
                                                           ".",
                                                           tr("Файл настроек *.ini (*.ini)"));

    QFile importFile(importFilePath);

    if (!importFile.open(QIODevice::ReadOnly))
    {

        QMessageBox::critical(this, tr("Error"), tr("Can't open ini file for read."));
        return;

    }

    QTextCodec * codec = QTextCodec::codecForName("Windows-1251");
    QString importText = codec -> toUnicode(importFile.readAll());

    importFile.close();

    //here some magic:
    importText = importText.replace("\\", "\\\\");
    importText = importText.replace(";", "\";\"");

    QTemporaryFile ptemp;

    if (!ptemp.open())
    {

        QMessageBox::critical(this, tr("Error"), tr("Can't open temp file for write."));
        return;

    }

    //Good. Now we can make voodoo:
    QString sDefaultCommand = "ping";

    ptemp.write(importText.toLocal8Bit());
    ptemp.close();

    QSettings importSettings(ptemp.fileName(), QSettings::IniFormat);

    importSettings.setIniCodec("Windows-1251");

    importSettings.beginGroup(QString::fromLatin1(codec -> fromUnicode("Общие")));
    QString sFiles = importSettings.value(QString::fromLatin1(codec -> fromUnicode("ФайлIP")), "").toString();

    QStringList aFiles = sFiles.split(';');

    for (int i = 0; i < aFiles.size(); ++i)
    {
        ui -> lvFileList -> addItem(aFiles.at(i));
    }

    if (
importSettings.value(QString::fromLatin1(codec -> fromUnicode("Автозапуск")), "").toBool() &&
       (QMessageBox::question(this,
                                tr("Автозагрузка"),
                                tr("Добавить приложение в автозапуск?"),
                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes))
    {
            addApptoStartUp();
    }

    sDefaultCommand = importSettings.value(QString::fromLatin1(codec -> fromUnicode("Ping")), "").toString();

    importSettings.endGroup();

    importSettings.beginGroup(QString::fromLatin1(codec -> fromUnicode("Commands")));

    QStringList aCommands = importSettings.childKeys();
    QString strTmp = "";

    for (int i = 0; i < aCommands.size(); ++i)
    {
        ui -> tvCommandsList -> setRowCount(ui -> tvCommandsList -> rowCount() + 1);
        ui -> tvCommandsList -> setItem(ui -> tvCommandsList -> rowCount() - 1, 1,
                                         new QTableWidgetItem(codec -> toUnicode(aCommands.at(i).toLatin1())));
        strTmp = importSettings.value(aCommands.at(i)).toString();

        strTmp.truncate(strTmp.indexOf(QString("|")));
        ui -> tvCommandsList -> setItem(ui -> tvCommandsList -> rowCount() - 1, 2,
                                         new QTableWidgetItem(strTmp));

        strTmp = importSettings.value(aCommands.at(i)).toString();
        strTmp = strTmp.right(strTmp.size() - strTmp.indexOf(QString("|")) - 1);
        ui -> tvCommandsList -> setItem(ui -> tvCommandsList -> rowCount() - 1, 3,
                                         new QTableWidgetItem(strTmp));

        if (codec -> toUnicode(aCommands.at(i).toLatin1()) == sDefaultCommand)
        {
            setDefaultCommandInCmdList(i);
        }
    }

    importSettings.endGroup();
    #else
        QMessageBox::information(this, tr("Info"), tr("Unavailable in *nix"));
    #endif
}

void SetSettings::updateAboutText()
{
    QString text = "ClipIp<br>"
                 "<br>"
                 "Версия: " + qApp -> applicationVersion() + " - пре-альфа-3<br>"
                 "<br>"
                 "Программа распространяется под лицензией GNU LGPL version 3<br>"
                 "<a href='http://www.gnu.org/licenses/lgpl-3.0.html' target='_blank'>http://www.gnu.org/licenses/lgpl-3.0.html</a><br>"
                 "<br>"
                 "Исходный код программы доступен по адресу:<br>"
                 "<a href='https://github.com/Wonkakakto/IpClip' target='_blank'>https://github.com/Wonkakakto/IpClip</a><br>"
                 "<br>"
                 "Файл с настройками приложения:<br>" + settingsApp -> fileName();
    ui -> tbAboutText -> clear();
    ui -> tbAboutText -> insertHtml(text);
}

void SetSettings::addApptoStartUp()
{
    #ifdef Q_OS_WIN32

        QString fullPathToExecuteFile = QApplication::applicationFilePath();
        WCHAR startupMenuPathForCurrentUser[MAX_PATH];
        HRESULT result = SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, 0, startupMenuPathForCurrentUser);

        if (SUCCEEDED(result))
        {

            QFile f;

            f.setFileName(QApplication::applicationFilePath());
            f.link(QDir::toNativeSeparators(QDir(QString::fromWCharArray(startupMenuPathForCurrentUser)).absolutePath())
                   + QDir::separator()
                   + QApplication::applicationName()
                   + ".lnk");
            QMessageBox::information(this,tr("Info"),tr("App added to startup."));

        } else {

            QMessageBox::critical(this,"Error","Can't find startup catalog.");

        }
    #else
        QMessageBox::information(this, tr("Not worked:"), tr("This function not work in *nix."));
    #endif
}

void SetSettings::on_pbStartup_clicked()
{

    addApptoStartUp();

}
