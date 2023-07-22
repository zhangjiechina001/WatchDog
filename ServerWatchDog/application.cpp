#include "application.h"
#include <QDebug>
#include <QDataStream>
#include <QBuffer>
#include <QProcess>
#include <QFileInfo>
#include <QSettings>
#include <QTextCodec>
#include <QDir>
#include <QFile>
#include "cleanmem.h"
#include "license.h"

#define APPLICATION_NAME    "LIBS_App_Server.exe"
#define APPLICATION_KEY     "LIBSAppServerAlreadyRunning"
#define APP_WATCHDOG_CYCLE  25
#define SHUT_DOWN_APP_CODE  126

Application::Application(int &argc, char **argv) :
    QApplication(argc, argv)
  , m_process(0)
  , m_systemTrayIcon(QIcon(":/dog.png"), this)
  , m_crashCount(0)
  , m_isRestarting(false)
{
    timeCount = 0;
    int checkInterval = 5000;
    QString settingPath = QCoreApplication::applicationDirPath() + "/serverwacthdog.ini";
    QSettings settings(settingPath, QSettings::IniFormat);
    QTextCodec *tc = QTextCodec::codecForName("utf-8");
    settings.setIniCodec(tc);
    settings.beginGroup("log");
    m_interval = settings.value("clearInterval", 1).toInt();
    settings.setValue("clearInterval", m_interval);
    checkInterval = settings.value("checkInterval", 10000).toInt();
    settings.setValue("checkInterval", checkInterval);
    settings.endGroup();
    m_interval = m_interval * 3600 * 12 / 5;

    qDebug()<<"~~~~~~~~~~~~~~~~~~~start~~~~~~~~~~~~~~~~~~~~~~~~";
    qDebug()<<"watchDog begin work for "<<APPLICATION_NAME<<" ... ";
    m_process.setWorkingDirectory(QCoreApplication::applicationDirPath() + "/");
    m_process.setProgram(m_process.workingDirectory() + APPLICATION_NAME);
    QFile f(m_process.workingDirectory() + APPLICATION_NAME);
    if (!f.exists()) {
        QString str = QString("Program file %1%2 is not exists.")
                .arg(m_process.workingDirectory()).arg(APPLICATION_NAME);
        qDebug()<< str;
        quit();
        return;
    }
    m_process.closeReadChannel(QProcess::StandardOutput);
    m_process.close();

    mem.setKey(APPLICATION_KEY);
    m_timerId = startTimer(checkInterval);
    m_systemTrayIcon.show();
}

bool Application::startProgram()
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

void Application::periodDetecte()
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
            startProgram();
            qDebug()<<"try to start application agin. ";
        } else {
            qDebug()<<"attach the shared memory failed!"<< mem.error() << mem.errorString();
           // printMemory();
        }
    }
    clearLogs();
}

Application::~Application()
{
    if (m_timerId != -1) {
        killTimer(m_timerId);
    }
    qDebug() << "watchdog ~Application";
}

void Application::clearLogs()
{
    timeCount++;
    if (timeCount > m_interval) {// 一个星期清除log
        timeCount = 0;
        qDebug() << "clearDog";
        QDir dir(QCoreApplication::applicationDirPath() + "/logs");
        dir.setFilter(QDir::Files);
        QDateTime curtime = QDateTime::currentDateTime();
        QDateTime fctime;
        if (dir.exists()) {
            foreach(QFileInfo fi ,dir.entryInfoList()) {
                if (fi.fileName().endsWith( ".log")) {
                    fctime = fi.created();
                    if (fctime.daysTo(curtime) > 7) {
                        qDebug() << "out filename:" << fi.absoluteFilePath();
                        dir.remove(fi.fileName());
                    }
                }
            }
        }

        dir.setPath(QCoreApplication::applicationDirPath());
        if (dir.exists()) {
            foreach(QFileInfo fi ,dir.entryInfoList()) {
                if (fi.fileName().endsWith( ".log")) {
                    fctime = fi.created();
                    if (fctime.daysTo(curtime) > 7) {
                        qDebug() << "out filename:" << fi.absoluteFilePath();
                        dir.remove(fi.fileName());
                    }
                }
            }
        }
    }
}

void Application::timerEvent(QTimerEvent *event)
{
    periodDetecte();
}

