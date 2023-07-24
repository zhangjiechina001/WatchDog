#pragma execution_character_set("utf-8")
#ifndef WATCHDOGITEM_H
#define WATCHDOGITEM_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QDateTime>
#include <QTimer>

class WatchDogItem : public QObject
{
    Q_OBJECT

public:
    enum Status
    {
        Off = 0,
        Block = 1,
        Running=2,
    };
    Q_ENUM(Status)

    WatchDogItem(QObject *parent=nullptr);
    ~WatchDogItem();
    void WaitForEnd();

    void SetConfig(QJsonObject obj);

    void SetName(QString name);
    QString Name();

    void InitProcess(QString filePath);
    bool StartProgram();
signals:
    void StatusChanged(Status currentStatus);

public slots:
    void periodDetecte();

private:
    QProcess  m_process;

    QDateTime   m_startTime;
    int timeCount;
    volatile  bool m_isRestarting;
    QTimer _timer;

    QString _programPath;
    QString _memoryKey;
    QSharedMemory _mem;
    QString _name;

    bool SetMemData(int val);
    int GetMemData();
};

#endif // WATCHDOGITEM_H
