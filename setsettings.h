#ifndef SETSETTINGS_H
#define SETSETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QTableWidgetItem>
#include "editcommand.h"

namespace Ui {
class SetSettings;
}

class SetSettings : public QDialog
{
    Q_OBJECT

public:
    explicit SetSettings(QWidget *parent = 0);
    ~SetSettings();

private slots:
    void on_pbAddFile_clicked();
    void on_pbDeleteFile_clicked();
    void on_pbSetDefaults_clicked();
    void on_buttonBox_accepted();
    void writeSettings();
    void readSettings();

    void on_pbAddCommand_clicked();

    void on_pbEditCommand_clicked();


    void on_pbDeleteCommand_clicked();

    void on_bpDefaultCommand_clicked();

public slots:
    void recieveAppSettings(QSettings *sp);
    void receiveCommandParam(QString sNameCmd,QString sFileNameExec,QString sParams,bool bNewCommand);

private:
    Ui::SetSettings *ui;
    QSettings *settingsApp;
    editCommand *editCommandWindow;

signals:
    void sendCommandSettings(QString sNameCmd,QString sFileNameExec,QString sParams,bool bNewCommand);
};

#endif // SETSETTINGS_H
