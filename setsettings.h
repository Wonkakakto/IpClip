#ifndef SETSETTINGS_H
#define SETSETTINGS_H

#include "editcommand.h"

//#include <QDialog>
#include <QSettings>
//#include <QTableWidgetItem>
//#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTemporaryFile>
#include <QStandardPaths>

#ifdef Q_OS_WIN32
#include <ShlObj.h>
#endif

namespace Ui {
class SetSettings;
}

class SetSettings : public QDialog
{
    Q_OBJECT

public:
    explicit SetSettings(QWidget* parent = 0);
    ~SetSettings();

private:
    Ui::SetSettings* ui;
    QSettings* settingsApp;
    editCommand* editCommandWindow;

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
    void on_pbUpCommand_clicked();
    void on_pbDownCommand_clicked();
    void on_lvFileList_itemSelectionChanged();
    void on_tvCommandsList_itemSelectionChanged();
    void on_pbUpFile_clicked();
    void on_pbDownFile_clicked();
    void cleanSettings();
    void setDefaultCommandInCmdList(int row);
    void addApptoStartUp();
    void updateAboutText();
    void on_pbImportSettings_clicked();

    void on_pbStartup_clicked();

public slots:
    void recieveAppSettings(QSettings* sp);
    void receiveCommandParam(QString sNameCmd, QString sFileNameExec, QString sParams, bool bNewCommand);

signals:
    void sendCommandSettings(QString sNameCmd, QString sFileNameExec, QString sParams, bool bNewCommand);

};

#endif // SETSETTINGS_H
