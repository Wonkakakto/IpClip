#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "setsettings.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QStandardPaths>
#include <QClipboard>
//#include <QSettings>
//#include <QStringList>
//#include <QMessageBox>
//#include <QCloseEvent>
//#include <QDir>
//#include <QProcess>
//#include <QKeySequence>
#include <QTextStream>


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
    void showWindowSetSettings();
    void setTrayIconActions();
    void showTrayIcon();
    void on_pbClose_clicked();
    void writeSettings();
    void readMainWindowPositionAndSize();
    void quitFromApp();
    void readIpList();
    void readCommandsList();
    void on_cbFind_currentTextChanged(const QString &arg1);
    void on_bpDoIt_customContextMenuRequested(const QPoint &pos);
    void on_bpDoIt_clicked();
    void on_pbIP2Buf_clicked();
    QString getIPfromStr();
    void onShowTrayMenu();
    void restoreWindow();
    void copyName2Buffer();
    void on_pbIP2Buf_customContextMenuRequested(const QPoint &pos);
    void on_pbClose_customContextMenuRequested(const QPoint &pos);
    void makeDoIt(QString cmdName);
    void onTrayIconActivate(QSystemTrayIcon::ActivationReason r);

private:
    Ui::MainWindow* ui;
    QMenu trayIconMenu;
    QAction minimizeAction;
    QAction restoreAction;
    QAction quitAction;
    QAction showWindowSetSettingsAction;
    QAction makeUpdate;
    QSystemTrayIcon trayIcon;
    QMenu commandMainMenu;
    QMenu ip2bufMenu;
    QAction name2bufferAction;
    QMenu closeButtonMenu;
    QAction closeAppAction;
    QSettings* settingsApp;
    SetSettings* SetSettingsDialog;
    QStringList ipList;
    QString defaultCommand[2];

protected slots:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent * event);

signals:
    void sendAppSettings(QSettings *sp);
};

#endif // MAINWINDOW_H
