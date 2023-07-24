#include "watchdogitem.h"
#include <QFile>
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>

#define APP_WATCHDOG_CYCLE  25
#define SHUT_DOWN_APP_CODE  126

WatchDogItem::WatchDogItem(QObject *parent) : QObject(parent),
    _timer(),
    m_systemTrayIcon(QIcon(":/dog.png"), this),
    mem("LIBSAppServerAlreadyRunning")
{
    InitProgress();
    StartProgram();
    connect(&_timer,&QTimer::timeout,this,&WatchDogItem::periodDetecte);
    _timer.setInterval(5*1000);
    _timer.start();
}

void WatchDogItem::InitProgress()
{
    QString filePath = "E:\\Projects\\WatchDog\\Demo\\bin_vs\\Demo.exe";
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << QString("Program file %1 does not exist.").arg(filePath);
    }

    m_process.setWorkingDirectory("E:/Projects/WatchDog/bin_vs/");
    m_process.setProgram(filePath);
    m_process.closeReadChannel(QProcess::StandardOutput);
}

bool WatchDogItem::StartProgram()
{
    bool appIsRunning = false;
    if (mem.create(1)) {
        qDebug() << "create mem";
        mem.detach();
    } else {
        if (mem.error() == QSharedMemory::AlreadyExists) {
            appIsRunning = true;
        } else {
            qDebug()<<"create shared memory error: "<<mem.error()<<mem.errorString() ;
        }
    }
    if (!appIsRunning) {
        m_process.start(QProcess::WriteOnly);
        appIsRunning = true;
        m_startTime = QDateTime::currentDateTime();
        m_systemTrayIcon.setToolTip(QStringLiteral("LIBS_App_Server程序"));
        m_systemTrayIcon.show();
        m_systemTrayIcon.showMessage(QStringLiteral("开始运行"), QStringLiteral("LIBS_App_Server程序"));
    } else {
        qDebug()<<"start Program failed, because the program is not exit or already running.";
    }
    return appIsRunning;
}

void WatchDogItem::periodDetecte()
{
    if (mem.attach()) {
        mem.lock();
        char* data = (char*)mem.data();
        int count = data[0]++;
        if (timeCount % 20 == 0) {
            qDebug() << "cout:" << count;
        }

        mem.unlock();
        if (m_process.state() == QProcess::Running) {
            mem.detach();
            // 进程在运行，但是程序已经阻塞,此时需要重启程序
            if (count > APP_WATCHDOG_CYCLE) {
                qCritical()<<"application non response deteceted. so we will kill and restart it.";
                m_isRestarting = true;
                m_process.kill();
                m_process.waitForFinished();
                m_isRestarting = false;
            }
        } else if (m_process.state() == QProcess::NotRunning) { // 程序不由看门狗控制，此时让程序自己关掉
            mem.lock();
            data[0] = SHUT_DOWN_APP_CODE;
            mem.unlock();
            mem.detach();
        }
    } else {
        if (mem.error() == QSharedMemory::NotFound) {
            StartProgram();
            qDebug()<<"try to start application agin. ";
        } else {
            qDebug()<<"attach the shared memory failed!"<< mem.error() << mem.errorString();
           // printMemory();
        }
    }
}
