#include "editcommand.h"
#include "ui_editcommand.h"

editCommand::editCommand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editCommand)
{
    ui->setupUi(this);
}

editCommand::~editCommand()
{
    delete ui;
}

void editCommand::on_buBoxOkCancel_accepted()
{
    this->accept();
}

void editCommand::receiveCommandSettings(QString sNameCmd,QString sFileNameExec,QString sParams)
{
    ui->leNameCmd->setText(sNameCmd);
    ui->leFileNameExec->setText(sFileNameExec);
    ui->lePrams->setText(sParams);
}

void editCommand::on_buBoxOkCancel_rejected()
{
    this->reject();
}
