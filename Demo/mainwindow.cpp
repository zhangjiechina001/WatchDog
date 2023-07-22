#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

QSharedMemory MainWindow::m_sharedMem("LIBSAppServerAlreadyRunning");

void MainWindow::CheckSharedMem()
{
    m_sharedMem.lock();
    char* data = (char*)m_sharedMem.data();
    if (data[0] == 126) {
        m_sharedMem.unlock();
        this->close();
    } else {
        data[0] = 0;
        m_sharedMem.unlock();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool isCreated=m_sharedMem.create(1);
    qDebug()<<__FUNCTION__<<__LINE__<<isCreated;
    _timer=new QTimer(this);
    _timer->setInterval(3000);
    connect(_timer,&QTimer::timeout,this,[this]()
    {
        CheckSharedMem();
    });
    CheckSharedMem();
    _timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
