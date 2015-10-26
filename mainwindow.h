#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QStringList>
#include "setsettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void changeEvent(QEvent*);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionExecute();
    void setTrayIconActions();
    void showTrayIcon();
    void on_pbClose_clicked();
    void showWindowSetSettings();
    void writeSettings();
    void readSettings();
    void quitFromApp();
    void readIpList();

    void on_cbFind_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *showWindowSetSettingsAction;
    QSystemTrayIcon *trayIcon;
    QSettings *settingsApp;
    SetSettings *SetSettingsDialog;
    QStringList *ipList;

protected slots:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent * event);
    //void doSendAppSettings();

signals:
    void sendAppSettings(QSettings *sp);
};

#endif // MAINWINDOW_H
