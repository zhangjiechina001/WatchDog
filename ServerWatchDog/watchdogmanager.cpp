#include "watchdogmanager.h"

#include <QJsonObject>
#include <QDebug>

#include "jsonutils.h"

SimpleWatchDogItem * WatchDogManager::CreateItem(QString type)
{
    SimpleWatchDogItem *tmpItem=nullptr;
    if(type=="WatchDogItem")
    {
        tmpItem=new WatchDogItem();
    }
    else if(type=="SimpleWatchDogItem")
    {
        tmpItem=new SimpleWatchDogItem();
    }
    return tmpItem;
}

WatchDogManager::WatchDogManager(QObject *parent) : QObject(parent)
{
    QJsonObject obj;
    JsonUtils::LoadJsonObject("WatchDogManager",obj);

    QJsonObject items=obj["Items"].toObject();
    for(auto key:items.keys())
    {
        QJsonObject objItem=items[key].toObject();
        SimpleWatchDogItem *tmpItem = CreateItem(objItem["Type"].toString());
        tmpItem->SetConfig(objItem);
        tmpItem->SetName(key);
        _watchDogItems.append(tmpItem);
        qDebug()<<__FUNCTION__<<__LINE__<<key<<"初始化完成";
    }
    AppUtils::AutoRunWithSystem(obj["PowerOnAutoRun"].toBool());
    _configObj=items;
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

SimpleWatchDogItem *WatchDogManager::GetItem(QString key)
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
