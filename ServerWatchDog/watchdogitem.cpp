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


WatchDogItem::WatchDogItem(QObject *parent):QObject(parent),
    m_process(),
    _timer()
{
}

WatchDogItem::~WatchDogItem()
{
}

void WatchDogItem::WaitForEnd()
{
    _timer.stop();
    _mem.detach();
    m_process.kill();
    m_process.waitForFinished();
    qDebug()<<__FUNCTION__<<__LINE__;
}


void WatchDogItem::SetConfig(QJsonObject obj)
{
    InitProcess(obj["ProgramPath"].toString());
    _mem.setKey(obj["MemoryKey"].toString());
    qDebug()<<__FUNCTION__<<__LINE__<<_mem.create(1);

    connect(&_timer,&QTimer::timeout,this,&WatchDogItem::periodDetecte);
    _timer.setInterval(obj["Internal"].toInt()*1000);
    _timer.start();
}

void WatchDogItem::SetName(QString name)
{
    _name=name;
}

QString WatchDogItem::Name()
{
    return _name;
}

void WatchDogItem::InitProcess(QString filePath)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        qDebug() << QString("Program file %1 does not exist.").arg(filePath);
        throw QString("Program file %1 does not exist.");
    }

    m_process.setWorkingDirectory(fileInfo.dir().absolutePath());
    m_process.setProgram(filePath);
    m_process.closeReadChannel(QProcess::StandardOutput);
}

bool WatchDogItem::StartProgram()
{
    m_process.start(QProcess::WriteOnly);
    m_startTime = QDateTime::currentDateTime();
    qDebug()<<__FUNCTION__<<__LINE__<<m_startTime;
    return false;
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
