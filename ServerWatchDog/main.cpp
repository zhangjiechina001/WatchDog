#include <QSharedMemory>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QApplication>
#include "mainwindow.h"
#include <Log/logger.h>


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
