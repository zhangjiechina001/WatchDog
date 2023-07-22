#include <QSharedMemory>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QApplication>
#include "mainwindow.h"

//static void AutoRunWithSystem(bool bAutoRun)
//{
//    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
//    if (bAutoRun) {
//        QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/watchDog.exe");
//        qDebug() << "strAppPath:" << strAppPath;
//        reg.setValue("LiveCamera_AutoRun", strAppPath);
//    } else {
//        reg.setValue("LiveCamera_AutoRun", "");
//    }
//}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
    win.show();
    return a.exec();
}
