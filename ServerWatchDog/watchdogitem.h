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
    explicit WatchDogItem(QObject *parent = nullptr);

    void InitProgress();
    bool StartProgram();
signals:

public slots:
    void periodDetecte();

private:
    QProcess  m_process;
    QSharedMemory mem;
    QDateTime   m_startTime;
    QSystemTrayIcon  m_systemTrayIcon;
    int timeCount;
    volatile  bool m_isRestarting;
    QTimer _timer;
};

#endif // WATCHDOGITEM_H
