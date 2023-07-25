#include <QSharedMemory>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QApplication>
#include "mainwindow.h"

static void AutoRunWithSystem(bool bAutoRun)
{
    // 获取当前程序路径
    QString appPath = QApplication::applicationFilePath();

    // 将当前程序添加到开机启动项
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    settings.setValue("MyApp", appPath);
    qDebug()<<"auto start:"<<appPath;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 设置应用程序图标
    QIcon icon(":/dog.ico");
    QApplication::setWindowIcon(icon);

    AutoRunWithSystem(true);
    //创建共享内存,判断是否已经运行程序
//    QSharedMemory mem("cameraWatchDogAlreadyRunning");
//    if (!mem.create(1)) {
//        return 0;
//    }
//    Application a(argc, argv);

//    a.setApplicationName("cameraWatchDog");
//    a.setApplicationDisplayName("cameraWatchDog");

//    QString path = QCoreApplication::applicationDirPath() + "/LiveIpCamera.ini";
//    QSettings settings(path, QSettings::IniFormat);
//    QTextCodec *tc = QTextCodec::codecForName("utf-8");
//    settings.setIniCodec(tc);
//    settings.beginGroup("run");
//    bool autoRun = settings.value("watchdogautoRun", false).toBool();
//    if (autoRun) {
//         AutoRunWithSystem(false); //程序自启动
//         settings.setValue("watchdogautoRun", false);
//    }else {
//        AutoRunWithSystem(false);
//   }
//    settings.endGroup();
//    a.startProgram();

    MainWindow win;
    win.setWindowIcon(icon);
    win.show();
    return a.exec();
}
