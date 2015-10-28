#include "editcommand.h"
#include "ui_editcommand.h"

editCommand::editCommand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editCommand)
{
    ui->setupUi(this);
    bNewCommand = true;
//    connect(this, SIGNAL(sendCommandSettings(QString,QString,QString,bool)),
//    editCommandWindow, SLOT(receiveCommandSettings(QString,QString,QString,bool)));
    connect(this, SIGNAL(sendCommandParam(QString,QString,QString,bool)),
            parent,SLOT(receiveCommandParam(QString,QString,QString,bool)));
}

editCommand::~editCommand()
{
    delete ui;
}

void editCommand::on_buBoxOkCancel_accepted()
{
    //samle: emit sendCommandSettings("test","test","test",false);
    emit sendCommandParam(ui->leNameCmd->text(),ui->leFileNameExec->text(),ui->lePrams->text(),this->bNewCommand);
    this->accept();
}

void editCommand::receiveCommandSettings(QString sNameCmd,QString sFileNameExec,QString sParams,bool bNewCmd)
{
    ui->leNameCmd->setText(sNameCmd);
    ui->leFileNameExec->setText(sFileNameExec);
    ui->lePrams->setText(sParams);
    this->bNewCommand = bNewCmd;
}

void editCommand::on_buBoxOkCancel_rejected()
{
    this->reject();
}
