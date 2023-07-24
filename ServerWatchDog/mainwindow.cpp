#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "Log/logger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qInstallMessageHandler(Logger::CustomMessageHandler);
    connect(&Logger::Instance(),SIGNAL(OnLogging(QString)),this,SLOT(Log(QString)));
    _watchDog=new WatchDogItem();
    _trayIcon=CreateTrayIcon();
    _trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete _watchDog;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (_trayIcon->isVisible())
    {
        qDebug()<<__LINE__;
        QIcon icon(":/dog.png");
        _trayIcon->showMessage("提示", "程序仍在后台运行！", icon,1000);
        hide();
        event->ignore();
    }
}

QSystemTrayIcon *MainWindow::CreateTrayIcon()
{
    QMenu *trayIconMenu=new QMenu(this);
    QAction *minimizeAction = new QAction(tr("&Minimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);
    trayIconMenu->addAction(minimizeAction);

    QAction *restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    QAction *quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, this, [=]()
    {
        _trayIcon->setVisible(false);
        this->close();
        qApp->quit();
        qDebug()<<"程序关闭！";
    });
    trayIconMenu->addAction(quitAction);

    auto trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/dog.png"));
    trayIcon->setToolTip("看门狗程序");
    return trayIcon;
}

void MainWindow::Log(QString log)
{
    ui->txtLog->appendPlainText(log);
    qDebug()<<log;
    if(ui->txtLog->document()->lineCount()>500)
    {
        ui->txtLog->document()->clear();
    }
}
