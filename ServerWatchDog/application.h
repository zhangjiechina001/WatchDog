#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QElapsedTimer>
#include <QWebSocket>
#include <QUrl>
#include <QTimer>
#include <QDateTime>
#include <QProcess>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QSharedMemory>

#define  HEADER_DATA 0xABCD1234
#define  TAIL_DATA   0x5678ABCD

typedef struct ClientPCInfoTag {
    qint32 header;
    qint32 lenght;
    qint64 currentTimeStamp;
    qint64 startTimeStamp;
    qint32 hostIP;
    qint32 hostPort;
    int    crashCount;
    float  cpuLoad;
    qint64 totalPhyMem;
    qint64 freePhyMem;
    qint64 totalPagingFile;
    qint64 freePagingFile;
    qint64 totalVirtualMem;
    qint64 freeVirtualMem;
    qint64 pageFaultCount;
    qint64 peakWorkingSize;
    qint64 workingSize;
    qint64 pageFileUsage;
    qint64 peakFileUsage;
    qint32 pad;
    qint32 tail;
}ClientPCInfo;

class Application : public QApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char **argv);
    ~Application();
public:
    bool startProgram();
private slots:
    void periodDetecte();
private:
    void clearLogs();
    bool checkLicense() ;
protected:
    void timerEvent(QTimerEvent *event);
private:
    QProcess   m_process;
    QSystemTrayIcon  m_systemTrayIcon;
    QDateTime   m_startTime;
    int         m_crashCount;
    volatile  bool m_isRestarting;
    int         m_interval;
    int         timeCount;
    int         m_timerId;
    QSharedMemory mem;
};

#endif // MAINWINDOW_H
