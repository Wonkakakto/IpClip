#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H

#include <QDialog>

namespace Ui {
class editCommand;
}

class editCommand : public QDialog
{
    Q_OBJECT

public:
    explicit editCommand(QWidget *parent = 0);
    ~editCommand();

public slots:
    void receiveCommandSettings(QString sNameCmd,QString sFileNameExec,QString sParams,bool bNewCmd);

private slots:
    void on_buBoxOkCancel_accepted();

    void on_buBoxOkCancel_rejected();

private:
    Ui::editCommand *ui;
    bool bNewCommand;

signals:
    void sendCommandParam(QString sNameCmd,QString sFileNameExec,QString sParams,bool bNewCommand);
};

#endif // EDITCOMMAND_H
