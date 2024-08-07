#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include <QObject>
#include <QJsonObject>

#include "watchdogitem.h"
#include "apputils.h"
#include "GlobalVar.h"

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

    SimpleWatchDogItem* GetItem(QString key);
    QList<QString> Keys();

signals:

public slots:

private:
    QList<SimpleWatchDogItem*> _watchDogItems;
    SimpleWatchDogItem *CreateItem(QString type);
};

#endif // WATCHDOGMANAGER_H
