#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <tuple>
#include "Log/logger.h"
#include "watchdogmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&Logger::Instance(),SIGNAL(OnLogging(QString)),this,SLOT(Log(QString)));
    _trayIcon=CreateTrayIcon();
    _trayIcon->show();

    WatchDogManager::Instance();
    for(QString key:WatchDogManager::Instance().Keys())
    {
        QLabel* label=AddLayout(key);
        WatchDogItem *watchDog=WatchDogManager::Instance().GetItem(key);
        connect(watchDog,&WatchDogItem::StatusChanged,this,[label](WatchDogItem::Status satus)
        {
            QMap<WatchDogItem::Status,QString> map={
                {WatchDogItem::Status::Off,"关闭,gray"},
                {WatchDogItem::Status::Block,"无响应,red"},
                {WatchDogItem::Status::Running,"运行,green"},
            };
            QString val=map[satus];
            label->setText(val.split(",").at(0));
            label->setStyleSheet(QString("background-color:%1;border: 1px solid black;qproperty-alignment: 'AlignCenter';").arg(val.split(",").at(1)));
        });
    }
    ui->verticalLayout->addItem(new QSpacerItem(20, 209, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

MainWindow::~MainWindow()
{
    WatchDogManager::Instance().deleteLater();
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
        WatchDogManager::Instance().WaitForEnd();
        qDebug()<<"程序关闭！";
    });
    trayIconMenu->addAction(quitAction);

    auto trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/dog2.ico"));
    trayIcon->setToolTip("看门狗程序");
    return trayIcon;
}

QLabel* MainWindow::AddLayout(QString name)
{
    auto *hLayout = new QHBoxLayout();
    QLabel *lblStatus = new QLabel(ui->widget);
    lblStatus->setText("在线");
    lblStatus->setMinimumSize(QSize(40, 20));
    lblStatus->setMaximumSize(QSize(40, 20));
    lblStatus->setStyleSheet(("background-color: green;border: 1px solid black;qproperty-alignment: 'AlignCenter';"));
    lblStatus->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    hLayout->addWidget(lblStatus);

    QLabel *lblText = new QLabel(ui->widget);
    lblText->setText(QString(" %1").arg(name));
    lblText->setLayoutDirection(Qt::LeftToRight);
    lblText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    hLayout->addWidget(lblText);

    ui->verticalLayout->addItem(hLayout);
    return lblStatus;
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
