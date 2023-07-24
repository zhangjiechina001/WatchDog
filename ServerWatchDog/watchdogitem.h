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
    WatchDogItem(QString programPath,QString memoryKey,QObject *parent=nullptr);
    void InitProgress(QString filePath);
    bool StartProgram();
signals:

public slots:
    void periodDetecte();

private:
    QProcess  m_process;
    QSharedMemory mem;
    QDateTime   m_startTime;
    int timeCount;
    volatile  bool m_isRestarting;
    QTimer _timer;

    QString _programPath;
    QString _memoryKey;
};

#endif // WATCHDOGITEM_H
