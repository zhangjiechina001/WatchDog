#pragma execution_character_set("utf-8")
#ifndef APPUTILS_H
#define APPUTILS_H

#include <QObject>

class AppUtils : public QObject
{
    Q_OBJECT
public:
    explicit AppUtils(QObject *parent = nullptr);

    static bool IsShareMemoryOccupy(const QString &appKey,bool showWarning);

    static bool IsProcessRunning(const QString &processName);

    static void WaitMs(int time);

private:
//    static QString sharedMemoryKey;

signals:

public slots:
};

#endif // APPUTILS_H
