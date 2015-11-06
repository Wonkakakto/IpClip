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
    void readCommandsList();
    void on_cbFind_currentTextChanged(const QString &arg1);
    void setMainMenuActions();
    void on_bpDoIt_customContextMenuRequested(const QPoint &pos);
    void on_bpDoIt_clicked();
    //void on_bpDoIt_Menu_clicked(QAction *event);

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
    QStringList *Commands[3];
    QString DefaultCommand[3];
    QMenu *commandMainMenu;

protected slots:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent * event);
    //void doSendAppSettings();

signals:
    void sendAppSettings(QSettings *sp);
};

#endif // MAINWINDOW_H
