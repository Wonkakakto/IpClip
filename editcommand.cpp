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
