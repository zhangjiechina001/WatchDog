#include <QSharedMemory>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QApplication>
#include "mainwindow.h"
#include <Log/logger.h>

static void AutoRunWithSystem(bool bAutoRun)
{
    // 获取当前程序路径
    QString appPath = QCoreApplication::applicationDirPath()+"/WatchDog.exe";
    // 将当前程序添加到开机启动项
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    appPath=appPath.replace("/","\\");
    settings.setValue("MyWatchDog", appPath);
    qDebug()<<"auto start:"<<appPath;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置为当前工作目录，要不然开机自启时会读不到配置文件
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    //日志记录
    qInstallMessageHandler(Logger::CustomMessageHandler);

    qDebug()<<"Auto Restart!";

    // 设置应用程序图标
    QIcon icon(":/dog.ico");
    QApplication::setWindowIcon(icon);

    MainWindow win;
    win.setWindowIcon(icon);
    win.show();
    return a.exec();
}
