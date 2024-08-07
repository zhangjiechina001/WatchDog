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

void SimpleWatchDogItem::SetConfig(QString appPath,int interval)
{
    InitProcess(appPath);
    connect(&_timer,&QTimer::timeout,this,&SimpleWatchDogItem::CheckStatus);
    _timer.setInterval(interval*1000);
    _timer.start();
}

void SimpleWatchDogItem::InitProcess(QString filePath)
{
    QFileInfo fileInfo(filePath);
     if (!fileInfo.exists()) {
         qDebug() << QString("Program file %1 does not exist.").arg(filePath);
         throw QString("Program file %1 does not exist.");
     }

     connect(&m_process, &QProcess::started, this, &SimpleWatchDogItem::onProcessStarted);
     connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &SimpleWatchDogItem::onProcessFinished);
     connect(&m_process, &QProcess::errorOccurred, this, &SimpleWatchDogItem::onProcessError);

     m_process.setWorkingDirectory(fileInfo.dir().absolutePath());
     m_process.setProgram(filePath);
     m_process.closeReadChannel(QProcess::StandardOutput);
}

bool SimpleWatchDogItem::StartProgram()
{
    m_process.start();
    qDebug()<<__FUNCTION__<<__LINE__<<QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
    return true;
}

void SimpleWatchDogItem::CheckStatus()
{
    qDebug()<<_name<<m_process.state();

    if (m_process.state() == QProcess::NotRunning)
    {
        emit StatusChanged(Status::Off);
        StartProgram();
    }
    else
    {
        emit StatusChanged(Status::Running);
    }
}

void SimpleWatchDogItem::onProcessStarted()
 {
     qDebug() << "Process started.";
 }

 void SimpleWatchDogItem::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
 {
     qDebug() << "Process finished with exit code" << exitCode << "and status" << exitStatus;
     // 这里可以根据 exitCode 和 exitStatus 做进一步的处理
 }

 void SimpleWatchDogItem::onProcessError(QProcess::ProcessError error)
 {
     qDebug() << "Process error occurred:" << error;
     // 可以在这里处理不同的错误类型，例如 QProcess::FailedToStart, QProcess::Crashed 等等
 }
