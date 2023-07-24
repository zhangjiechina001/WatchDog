#pragma execution_character_set("utf-8")
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "watchdogitem.h"
#include <QMainWindow>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QSystemTrayIcon* CreateTrayIcon();

private slots:
    void Log(QString log);

private:
    Ui::MainWindow *ui;
    WatchDogItem *_watchDog;
    QSystemTrayIcon  *_trayIcon=nullptr;
};

#endif // MAINWINDOW_H
