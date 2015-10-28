#include "editcommand.h"
#include "ui_editcommand.h"

editCommand::editCommand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editCommand)
{
    ui->setupUi(this);
    bNewCommand = true;
}

editCommand::~editCommand()
{
    delete ui;
}

void editCommand::on_buBoxOkCancel_accepted()
{
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
