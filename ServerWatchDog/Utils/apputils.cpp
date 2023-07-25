#include "apputils.h"
#include <QSharedMemory>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QSettings>

AppUtils::AppUtils(QObject *parent) : QObject(parent)
{

}


bool AppUtils::IsShareMemoryOccupy(const QString &appKey, bool showWarning)
{
    const QString& sharedMemoryKey=appKey;
    // 创建共享内存对象，并检查是否已存在
    static QSharedMemory sharedMemory(sharedMemoryKey);
    if (sharedMemory.attach()&&showWarning) {
        // 共享内存已存在，表示已经有一个实例在运行
        QMessageBox::warning(nullptr, "警告", "只允许创建一个实例!");
        return true;
    }

    // 将共享内存附加到当前进程
    if (!sharedMemory.create(1)&&showWarning) {
        // 无法创建共享内存
        QMessageBox::critical(nullptr, "Error", "Unable to create shared memory.");
        return true;
    }

    return false;
}

bool AppUtils::IsProcessRunning(const QString &processName)
{
    QProcess process;
    QString command = "tasklist";

    process.start("cmd.exe", QStringList() << "/C" << command);
    process.waitForFinished();

    QByteArray output = process.readAllStandardOutput();
    QString outputString(output);
    qDebug()<<outputString.split("\n");
    if(outputString.split("\n").count(processName)>1)
    {
        QMessageBox::warning(nullptr, "Warning", "Only one instance of the application is allowed.");
        return true;
    }
    return false;
}

void AppUtils::WaitMs(int time)
{
    qDebug()<<"开始等待"<<time/1000<<"s";
    QElapsedTimer ela;
    ela.start();
    while (ela.elapsed() < time)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 300);
    }
    qDebug()<<"结束等待";
}

void AppUtils::AutoRunWithSystem()
{
    // 获取当前程序路径
    QString appPath = QCoreApplication::applicationDirPath()+"/WatchDog.exe";
    // 将当前程序添加到开机启动项
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    appPath=appPath.replace("/","\\");
    settings.setValue("MyWatchDog", appPath);
    qDebug()<<"auto start:"<<appPath;
}

