#ifndef WATCHDOGITEM_H
#define WATCHDOGITEM_H

#include <QObject>
#include <QSharedMemory>

class WatchDogItem : public QObject
{
    Q_OBJECT
public:
    explicit WatchDogItem(QObject *parent = nullptr);

    static void InitProgress();
     void StartProgram();
signals:

public slots:

private:
     QSharedMemory mem;
};

#endif // WATCHDOGITEM_H
