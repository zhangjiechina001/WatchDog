#include "simplewatchdogitem.h"
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include <QDir>

SimpleWatchDogItem::SimpleWatchDogItem(QObject *parent) : QObject(parent),
    _timer()
{

}

void SimpleWatchDogItem::WaitForEnd()
{
    _timer.stop();
    m_process.kill();
    m_process.waitForFinished();
    qDebug()<<__FUNCTION__<<__LINE__;
}

void SimpleWatchDogItem::SetName(QString name)
{
    _name=name;
}

QString SimpleWatchDogItem::Name()
{
    return _name;
}

void SimpleWatchDogItem::SetConfig(QJsonObject obj)
{
    InitProcess(obj["ProgramPath"].toString());
    connect(&_timer,&QTimer::timeout,this,&SimpleWatchDogItem::CheckStatus);
    _timer.setInterval(obj["Internal"].toInt()*1000);
    _timer.start();
}

void SimpleWatchDogItem::InitProcess(QString filePath)
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

bool SimpleWatchDogItem::StartProgram()
{
    m_process.start(QProcess::WriteOnly);
    qDebug()<<__FUNCTION__<<__LINE__<<QDateTime::currentDateTime();;
    return true;
}

void SimpleWatchDogItem::CheckStatus()
{
    if (m_process.state() == QProcess::NotRunning)
    {
         qDebug()<<__FUNCTION__<<__LINE__<<m_process.state();
        emit StatusChanged(Status::Off);
        StartProgram();
    }
    else
    {
        emit StatusChanged(Status::Running);
    }
}
