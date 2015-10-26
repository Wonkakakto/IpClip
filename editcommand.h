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
    void receiveCommandSettings(QString sNameCmd,QString sFileNameExec,QString sParams);

private slots:
    void on_buBoxOkCancel_accepted();

    void on_buBoxOkCancel_rejected();

private:
    Ui::editCommand *ui;
};

#endif // EDITCOMMAND_H
