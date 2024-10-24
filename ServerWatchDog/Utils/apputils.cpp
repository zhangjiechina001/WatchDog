#include "apputils.h"
#include <QSharedMemory>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QSettings>
#include <QApplication>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>

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

void AppUtils::AutoRunWithSystem(bool autoStart)
{
    if(autoStart)
    {
        addAppToStartup();
    }
    else
    {
        removeAppFromStartup();
    }
}

// 添加应用程序到开机启动
void AppUtils::addAppToStartup()
{
    // 获取当前运行程序的路径
   QString targetPath = QCoreApplication::applicationFilePath();

   // 从资源文件读取批处理脚本
   QFile batchFile(":/mybat.bat");
   if (!batchFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qDebug() << "无法打开资源中的批处理文件。";
       return;
   }

   // 读取脚本内容
   QString scriptContent = batchFile.readAll();
   batchFile.close();

   // 替换占位符 %1 为实际的目标路径
   scriptContent.replace("%1", "\"" + targetPath + "\"");
   scriptContent.replace("%2", "\"" + QString("WatchDog") + "\"");

   // 创建临时批处理文件
   QTemporaryFile tempBatFile(QDir::temp().absoluteFilePath("CreateStartupShortcut.XXXXXX.bat"));
   if (!tempBatFile.open()) {
       qDebug() << "无法创建临时批处理文件。";
       return;
   }

   // 将修改后的脚本内容写入临时文件
   QTextStream out(&tempBatFile);
   out << scriptContent;

   // 关闭临时批处理文件
   tempBatFile.close();

   // 使用 QProcess 执行批处理文件
   QProcess process;
   process.start("cmd.exe", QStringList() << "/c" << tempBatFile.fileName());
   process.waitForFinished();

   // 可选：读取输出和错误信息（用于调试）
   QString output = process.readAllStandardOutput();
   QString error = process.readAllStandardError();
   if (!output.isEmpty()) {
       qDebug() << "输出：" << output;
   }
   if (!error.isEmpty()) {
       qDebug() << "错误：" << error;
   }
}

// 移除开机启动
void AppUtils::removeAppFromStartup()
{
    QString startupFolder = "C:/ProgramData/Microsoft/Windows/Start Menu/Programs/StartUp/";
    QString shortcutPath = startupFolder + QCoreApplication::applicationName() + ".lnk";

    QFile shortcutFile(shortcutPath);
    if (shortcutFile.exists()) {
        if (shortcutFile.remove()) {
            qDebug() << "Shortcut removed successfully.";
        } else {
            qDebug() << "Failed to remove shortcut.";
        }
    } else {
        qDebug() << "Shortcut does not exist.";
    }
}

