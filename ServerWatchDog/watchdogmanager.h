#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include <QObject>
#include <QJsonObject>

#include "watchdogitem.h"

class WatchDogManager : public QObject
{
    Q_OBJECT
private:
    explicit WatchDogManager(QObject *parent = 0);
    ~WatchDogManager();
    WatchDogManager(const WatchDogManager&)=delete;

public:
    static WatchDogManager& Instance();
    void WaitForEnd();

    WatchDogItem* GetItem(QString key);
    QList<QString> Keys();

signals:

public slots:

private:
    QList<WatchDogItem*> _watchDogItems;
    QJsonObject _configObj;
};

#endif // WATCHDOGMANAGER_H
