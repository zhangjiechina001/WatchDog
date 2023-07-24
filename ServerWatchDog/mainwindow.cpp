#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "Log/logger.h"
#include "watchdogmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qInstallMessageHandler(Logger::CustomMessageHandler);
    connect(&Logger::Instance(),SIGNAL(OnLogging(QString)),this,SLOT(Log(QString)));
    _trayIcon=CreateTrayIcon();
    _trayIcon->show();

    WatchDogManager::Instance();

    ui->verticalLayout->addItem(CreateLayout("LIBS2700服务端"));
    ui->verticalLayout->addItem(CreateLayout("LIBS2700控制端"));
    ui->verticalLayout->addItem(new QSpacerItem(20, 209, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

MainWindow::~MainWindow()
{
//    delete _watchDog;
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

QLayoutItem* MainWindow::CreateLayout(QString name)
{
    auto *hLayout = new QHBoxLayout();
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(1);

    QLabel *lblFeederStatus_2 = new QLabel(ui->widget);

    lblFeederStatus_2->setMinimumSize(QSize(20, 20));
    lblFeederStatus_2->setMaximumSize(QSize(20, 20));
    lblFeederStatus_2->setStyleSheet(("background-color: green;border-radius:10px;border: 1px solid black;"));
    sizePolicy.setHeightForWidth(lblFeederStatus_2->sizePolicy().hasHeightForWidth());
    lblFeederStatus_2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    hLayout->addWidget(lblFeederStatus_2);

    QLabel *lblFedderText_2 = new QLabel(ui->widget);
    lblFedderText_2->setText(QString(" %1").arg(name));
    lblFedderText_2->setLayoutDirection(Qt::LeftToRight);
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHeightForWidth(lblFedderText_2->sizePolicy().hasHeightForWidth());
    sizePolicy1.setHorizontalStretch(1);
    sizePolicy1.setVerticalStretch(1);
    sizePolicy1.setHeightForWidth(lblFedderText_2->sizePolicy().hasHeightForWidth());
    lblFedderText_2->setSizePolicy(sizePolicy1);
    hLayout->addWidget(lblFedderText_2);

    return hLayout;
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
