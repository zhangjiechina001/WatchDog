//#include "mainwindow.h"
//#include <QApplication>
//#include <WinSock2.h>

//#pragma comment (lib,"ws2_32.lib")
//inline int SQ(int x){return x*x;}

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    int a1=100;
//    int b1=200;
//    std::swap(a1,b1);

//    return a.exec();
//}

#include <QCoreApplication>
#include <QDebug>
#include <cstring>

static void QStringCopyCStrArray(const QString& source, char target[]) {
    qstrncpy(target, source.toUtf8().constData(), strlen(source.toUtf8().constData()));
    target[source.length()] = '\0'; // 添加终止字符 '\0'
}

void GetLambda(double* lambda)
{
    *lambda=100;
    lambda++;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    return app.exec();
}
