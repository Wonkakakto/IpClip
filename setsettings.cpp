#include "setsettings.h"
#include "ui_setsettings.h"
#include <QFileDialog>
#include <QMessageBox>

SetSettings::SetSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetSettings)
{
    ui->setupUi(this);
    editCommandWindow = new editCommand(this);
    connect(this, SIGNAL(sendCommandSettings(QString,QString,QString,bool)),
            editCommandWindow, SLOT(receiveCommandSettings(QString,QString,QString,bool)));

    ui->tvCommandsList->setColumnCount(4);
    ui->tvCommandsList->setHorizontalHeaderItem(0,new QTableWidgetItem(""));
    ui->tvCommandsList->setHorizontalHeaderItem(1,new QTableWidgetItem("Команда"));
    ui->tvCommandsList->setHorizontalHeaderItem(2,new QTableWidgetItem("Имя исполняемого файла"));
    ui->tvCommandsList->setHorizontalHeaderItem(3,new QTableWidgetItem("Параметры"));
    ui->tvCommandsList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvCommandsList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);//Fixed
    //ui->tvCommandsList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tvCommandsList->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    //ui->tvCommandsList->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);


    ui->tvCommandsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvCommandsList->setSelectionBehavior(QAbstractItemView::SelectRows);




#ifdef Q_OS_WIN32
   ui->cbAutostart->setEnabled(true);
#else
   ui->cbAutostart->setEnabled(false);
#endif

//WHILE DEVELOPE
   ui->cbSaveHistory->setEnabled(false);
   ui->cbSaveHistoryDepth->setEnabled(false);
   ui->pbSetDefaults->setEnabled(false);
   ui->pbImportSettings->setEnabled(false);
//
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
    settingsApp->setValue("SavePosition",ui->cbSaveMainWindowPosition->checkState());
    settingsApp->endGroup();

    QString defaultCommand = "Ping";
    settingsApp->remove("Commands");
    settingsApp->beginWriteArray("Commands");
    for(int j=0;j<ui->tvCommandsList->rowCount();++j)
    {
        settingsApp->setArrayIndex(j);
        settingsApp->setValue("CommandName",ui->tvCommandsList->item(j,1)->text());
        settingsApp->setValue("CommandPath",ui->tvCommandsList->item(j,2)->text());
        settingsApp->setValue("CommandParams",ui->tvCommandsList->item(j,3)->text());
        if(ui->tvCommandsList->item(j,0)!=0 )
            defaultCommand = ui->tvCommandsList->item(j,1)->text();
    }
    settingsApp->endArray();
    settingsApp->beginGroup("MainWindow");
    settingsApp->setValue("DefaultCommand", defaultCommand);
    settingsApp->endGroup();
}

void SetSettings::readSettings()
{
    QString defaultCommand = "Ping";
    settingsApp->beginGroup("MainWindow");
    defaultCommand = settingsApp->value("DefaultCommand","Ping").toString();
    settingsApp->endGroup();

    int size = settingsApp->beginReadArray("Files");
    for(int i=0; i<size; ++i)
    {
        settingsApp->setArrayIndex(i);
        ui->lvFileList->addItem(settingsApp->value("File").toString());
    }
    settingsApp->endArray();

    settingsApp->beginGroup("MainWindow");
    ui->cbSaveMainWindowPosition->setChecked(settingsApp->value("SavePosition",true).toBool());
    settingsApp->endGroup();


    size = settingsApp->beginReadArray("Commands");
    ui->tvCommandsList->setRowCount(size);
    for(int j=0;j<size;++j)
    {
        settingsApp->setArrayIndex(j);
        ui->tvCommandsList->setItem(j,1,new QTableWidgetItem(settingsApp->value("CommandName").toString()));
        ui->tvCommandsList->setItem(j,2,new QTableWidgetItem(settingsApp->value("CommandPath").toString()));
        ui->tvCommandsList->setItem(j,3,new QTableWidgetItem(settingsApp->value("CommandParams").toString()));
        if(defaultCommand == settingsApp->value("CommandName").toString())
            ui->tvCommandsList->setItem(j,0,new QTableWidgetItem(QPixmap(":/images/images/icon.png"),""));
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
    int row = ui->tvCommandsList->currentRow();
    if( row != -1 )
    {
        emit sendCommandSettings(ui->tvCommandsList->item(row,1)->text(),
                                 ui->tvCommandsList->item(row,2)->text(),
                                 ui->tvCommandsList->item(row,3)->text(),false);
        editCommandWindow->exec();
    }
}

void SetSettings::on_pbAddCommand_clicked()
{
    emit sendCommandSettings("","","",true);
    editCommandWindow->exec();
}

void SetSettings::receiveCommandParam(QString sNameCmd,QString sFileNameExec,QString sParams,bool bNewCommand)
{
    int row = 0;

    if( bNewCommand )
    {
        row = ui->tvCommandsList->rowCount();
        ui->tvCommandsList->setRowCount(row + 1);
        ui->tvCommandsList->setItem(row,1,new QTableWidgetItem(sNameCmd));
        ui->tvCommandsList->setItem(row,2,new QTableWidgetItem(sFileNameExec));
        ui->tvCommandsList->setItem(row,3,new QTableWidgetItem(sParams));

    }else
    {
        row = ui->tvCommandsList->currentRow();
        ui->tvCommandsList->item(row,1)->setText(sNameCmd);
        ui->tvCommandsList->item(row,2)->setText(sFileNameExec);
        ui->tvCommandsList->item(row,3)->setText(sParams);
    }
}

void SetSettings::on_pbDeleteCommand_clicked()
{
    int row = ui->tvCommandsList->currentRow();
    if( row != -1 )
    {
        ui->tvCommandsList->removeRow(row);
    }
}

void SetSettings::on_bpDefaultCommand_clicked()
{
    int row = ui->tvCommandsList->currentRow();
    if( row != -1 )
    {
        QTableWidgetItem* item;
        item = ui->tvCommandsList->item(row,0);
        if(item == 0)
            ui->tvCommandsList->setItem(row,0,new QTableWidgetItem(QPixmap(":/images/images/icon.png"),""));
        else
            item->setIcon(QPixmap(":/images/images/icon.png"));
        for(int i=0;i<ui->tvCommandsList->rowCount();++i)
            if(i!=row && ui->tvCommandsList->item(i,0)!=0 )
                delete ui->tvCommandsList->item(i,0);
    }
}

void SetSettings::on_pbUpCommand_clicked()
{
    int rowcount = ui->tvCommandsList->rowCount();
    int row = ui->tvCommandsList->currentRow();
    //if( row == -1 || row == 0 || rowcount < 2)
    if( row > 0 && rowcount > 1)
    {
        QTableWidgetItem* item;
        if( ui->tvCommandsList->item(row,0) != 0 || ui->tvCommandsList->item(row-1,0) != 0 )
            if( ui->tvCommandsList->item(row,0) != 0 )
            {
                ui->tvCommandsList->setItem(row-1,0,ui->tvCommandsList->takeItem(row,0));
                delete ui->tvCommandsList->item(row,0);
            }else
            {
                ui->tvCommandsList->setItem(row,0,item = ui->tvCommandsList->takeItem(row-1,0));
                delete ui->tvCommandsList->item(row-1,0);
            }
        for(int i=1;i<4;++i)
        {
            item = ui->tvCommandsList->takeItem(row,i);
            ui->tvCommandsList->setItem(row,i,ui->tvCommandsList->takeItem(row-1,i));
            ui->tvCommandsList->setItem(row-1,i,item);
        }
        ui->tvCommandsList->selectRow(row-1);
    }
}

void SetSettings::on_pbDownCommand_clicked()
{
    int rowcount = ui->tvCommandsList->rowCount();
    int row = ui->tvCommandsList->currentRow();
    // row != -1 and row != rowcount - 1 and rowcount > 1
    if( row != -1 && rowcount > 1 && row != rowcount - 1)
    {
        QTableWidgetItem* item;
        if( ui->tvCommandsList->item(row,0) != 0 || ui->tvCommandsList->item(row+1,0) != 0 )
            if( ui->tvCommandsList->item(row,0) != 0 )
            {
                ui->tvCommandsList->setItem(row+1,0,ui->tvCommandsList->takeItem(row,0));
                delete ui->tvCommandsList->item(row,0);
            }else
            {
                ui->tvCommandsList->setItem(row,0,item = ui->tvCommandsList->takeItem(row+1,0));
                delete ui->tvCommandsList->item(row+1,0);
            }
        for(int i=1;i<4;++i)
        {
            item = ui->tvCommandsList->takeItem(row,i);
            ui->tvCommandsList->setItem(row,i,ui->tvCommandsList->takeItem(row+1,i));
            ui->tvCommandsList->setItem(row+1,i,item);
        }
        ui->tvCommandsList->selectRow(row+1);
    }
}
