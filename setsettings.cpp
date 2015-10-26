#include "setsettings.h"
#include "ui_setsettings.h"
#include <QFileDialog>

SetSettings::SetSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetSettings)
{
    ui->setupUi(this);
    editCommandWindow = new editCommand;
    connect(this, SIGNAL(sendCommandSettings(QString,QString,QString)),
            editCommandWindow, SLOT(receiveCommandSettings(QString,QString,QString)));

#ifdef Q_OS_WIN32
   ui->cbAutostart->setEnabled(true);
#else
      ui->cbAutostart->setEnabled(false);
#endif
}

SetSettings::~SetSettings()
{
    delete this->editCommandWindow;
    delete ui;
}

void SetSettings::on_pbAddFile_clicked()
{
    ui->lvFileList->addItems(QFileDialog::getOpenFileNames(this, tr("Выбрать файл:"),".",tr("Файл со списком адресов *.txt (*.*)")));
}

void SetSettings::on_pbDeleteFile_clicked()
{
    delete ui->lvFileList->currentItem();
}

void SetSettings::on_pbSetDefaults_clicked()
{

}

void SetSettings::on_buttonBox_accepted()
{
    writeSettings();
}

void SetSettings::writeSettings()
{
    settingsApp->remove("Files");
    settingsApp->beginWriteArray("Files");
    for(int i=0;i<ui->lvFileList->count();++i)
    {
        settingsApp->setArrayIndex(i);
        settingsApp->setValue("File",ui->lvFileList->item(i)->text());
    }

    settingsApp->endArray();
    settingsApp->beginGroup("MainWindow");
    //settingsApp->value("saveposition",ui->cbSaveMainWindowPosition->checkState(),true)
    settingsApp->endGroup();
}

void SetSettings::readSettings()
{
    int size = settingsApp->beginReadArray("Files");
    for(int i=0; i<size; ++i)
    {
        settingsApp->setArrayIndex(i);
        ui->lvFileList->addItem(settingsApp->value("File").toString());
    }
    settingsApp->endArray();
}

void SetSettings::recieveAppSettings(QSettings *sp)
{
    this->settingsApp = sp;
    readSettings();
}

void SetSettings::on_pbEditCommand_clicked()
{
    emit sendCommandSettings("test","test","test");
    editCommandWindow->exec();
}

void SetSettings::on_pbAddCommand_clicked()
{
    emit sendCommandSettings("new","new","new");
    editCommandWindow->exec();
}
