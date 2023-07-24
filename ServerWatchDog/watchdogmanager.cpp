#include "watchdogmanager.h"

#include <QJsonObject>
#include <QDebug>

#include "jsonutils.h"

WatchDogManager::WatchDogManager(QObject *parent) : QObject(parent)
{
    QJsonObject obj;
    JsonUtils::LoadJsonObject("WatchDogManager",obj);

    for(auto key:obj.keys())
    {
        QJsonObject objItem=obj[key].toObject();
        WatchDogItem *tmpItem=new WatchDogItem();
        tmpItem->SetConfig(objItem);
        tmpItem->SetName(key);
        _watchDogItems.append(tmpItem);
        qDebug()<<__FUNCTION__<<__LINE__<<key<<"初始化完成";
    }
    _configObj=obj;
}

WatchDogManager::~WatchDogManager()
{

}

WatchDogManager &WatchDogManager::Instance()
{
    static WatchDogManager _instance;
    return _instance;
}

void WatchDogManager::WaitForEnd()
{
    for(auto item:_watchDogItems)
    {
        item->WaitForEnd();
    }
}

WatchDogItem *WatchDogManager::GetItem(QString key)
{
    for(auto item:_watchDogItems)
    {
        if(item->Name()==key)
        {
            return item;
        }
    }
    return nullptr;
}

QList<QString> WatchDogManager::Keys()
{
    return _configObj.keys();
}
