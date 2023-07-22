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
    qDebug()<<"~~~~~~~~~~~~~~~~~~~start~~~~~~~~~~~~~~~~~~~~~~~~";
    qDebug()<<"watchDog begin work for "<<APPLICATION_NAME<<" ... ";


    QString filePath = "E:\\Projects\\WatchDog\\Demo\\bin_vs\\Demo.exe";
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << QString("Program file %1 does not exist.").arg(filePath);
        QCoreApplication::quit();
    }

    m_process.setWorkingDirectory("E:/Projects/WatchDog/bin_vs/");
    m_process.setProgram(filePath);
    m_process.closeReadChannel(QProcess::StandardOutput);
    m_process.close();

    mem.setKey("LIBSAppServerAlreadyRunning");

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
}

Application::~Application()
{
    if (m_timerId != -1) {
        killTimer(m_timerId);
    }
    qDebug() << "watchdog ~Application";
}

void Application::timerEvent(QTimerEvent *event)
{
    periodDetecte();
}

