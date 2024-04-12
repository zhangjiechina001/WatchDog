#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <staticmemberdemo.h>

QSharedMemory MainWindow::m_sharedMem;

void MainWindow::CheckSharedMem()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    if(!m_sharedMem.attach())
    {
        qDebug()<<"看门狗未打开！";
        return;
    }
    m_sharedMem.lock();
    char* data = (char*)m_sharedMem.data();
    if (data[0] == 126) {
        this->close();
    } else {
        data[0] = 0;
    }
    m_sharedMem.detach();
    m_sharedMem.unlock();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_sharedMem.setKey("LIBSAppServerAlreadyRunning");
    _timer=new QTimer(this);
    _timer->setInterval(1000);
    connect(_timer,&QTimer::timeout,this,[this]()
    {
        CheckSharedMem();
    });
    _timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked)
    {
        _timer->start();
    }
    else
    {
         _timer->stop();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
//    StaticMemberDemo::Instance();
}
