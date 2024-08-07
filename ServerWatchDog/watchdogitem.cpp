#include "watchdogitem.h"
#include <QFile>
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>

#define APP_WATCHDOG_CYCLE  25
#define SHUT_DOWN_APP_CODE  126


WatchDogItem::WatchDogItem(QObject *parent):SimpleWatchDogItem(parent),
    m_process(),
    _timer()
{
}

WatchDogItem::~WatchDogItem()
{
}

void WatchDogItem::WaitForEnd()
{
    SimpleWatchDogItem::WaitForEnd();
    _mem.detach();
}


//void WatchDogItem::SetConfig(QJsonObject obj)
//{
//    SimpleWatchDogItem::SetConfig(obj);
//    _mem.setKey(obj["MemoryKey"].toString());
//    qDebug()<<__FUNCTION__<<__LINE__<<_mem.create(1);
//}

void WatchDogItem::CheckStatus()
{
    SimpleWatchDogItem::CheckStatus();
    if (_mem.isAttached()) {
        int count = GetMemData();
        SetMemData(count+1);
        emit StatusChanged(count>1?Status::Block:Status::Running);

        // 进程在运行，但是程序已经阻塞,此时需要重启程序
        if (count > APP_WATCHDOG_CYCLE)
        {
            SetMemData(0);
            qCritical()<<"application non response deteceted. so we will kill and restart it.";
            //程序不由看门狗控制，此时让程序自己关掉
            emit StatusChanged(Status::Off);
            m_process.kill();
            m_process.waitForFinished();
        }
    }
}

bool WatchDogItem::SetMemData(int val)
{
    _mem.lock();
    char* data = (char*)_mem.data();
    data[0]=val;
    _mem.unlock();
    return true;
}

int WatchDogItem::GetMemData()
{
    _mem.lock();
    char* data = (char*)_mem.data();
    int count = data[0];
    _mem.unlock();
    return count;
}

void WatchDogItem::periodDetecte()
{
    if (_mem.isAttached()) {
        int count = GetMemData();
        SetMemData(count+1);
        emit StatusChanged(count>1?Status::Block:Status::Running);

        if (m_process.state() == QProcess::Running)
        {
            // 进程在运行，但是程序已经阻塞,此时需要重启程序
            if (count > APP_WATCHDOG_CYCLE)
            {
                SetMemData(0);
                qCritical()<<"application non response deteceted. so we will kill and restart it.";
                m_process.kill();
                m_process.waitForFinished();
            }
        }
        else if (m_process.state() == QProcess::NotRunning)
        {
            //程序不由看门狗控制，此时让程序自己关掉
            emit StatusChanged(Status::Off);
            StartProgram();
        }
    }
    else
    {
        if (_mem.error() == QSharedMemory::NotFound)
        {
            StartProgram();
            qDebug()<<"try to start application agin. ";
        }
        else
        {
            qDebug()<<"attach the shared memory failed!"<< _mem.error() << _mem.errorString();
        }
    }
}
