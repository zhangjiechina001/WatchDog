#pragma execution_character_set("utf-8")
#ifndef WATCHDOGITEM_H
#define WATCHDOGITEM_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QDateTime>
#include <QTimer>
#include <simplewatchdogitem.h>

class WatchDogItem : public SimpleWatchDogItem
{
    Q_OBJECT

public:


    WatchDogItem(QObject *parent=nullptr);
    ~WatchDogItem();
    void WaitForEnd() override;

public slots:
    void periodDetecte();

private:
    QProcess  m_process;
    QDateTime m_startTime;
    int timeCount;
    QTimer _timer;

    QString _programPath;
    QString _memoryKey;
    QSharedMemory _mem;
    QString _name;

    bool SetMemData(int val);
    int GetMemData();

    // SimpleWatchDogItem interface
public slots:
    void SetConfig(QJsonObject obj) override;
    void CheckStatus() override;
};

#endif // WATCHDOGITEM_H
