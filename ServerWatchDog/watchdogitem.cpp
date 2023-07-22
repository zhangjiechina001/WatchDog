#include "watchdogitem.h"
#include <QFile>
#include <QProcess>
#include <QCoreApplication>

WatchDogItem::WatchDogItem(QObject *parent) : QObject(parent)
{

}

void WatchDogItem::InitProgress()
{
    QProcess   m_process;
    QString filePath = "E:\\Projects\\WatchDog\\Demo\\bin_vs\\Demo.exe";
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << QString("Program file %1 does not exist.").arg(filePath);
    }

    m_process.setWorkingDirectory("E:/Projects/WatchDog/bin_vs/");
    m_process.setProgram(filePath);
    m_process.closeReadChannel(QProcess::StandardOutput);
}

void WatchDogItem::StartProgram()
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
