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
    WatchDogParam p=GlobalVar::Instance().GetWatchDogParam();
    qDebug()<<__FUNCTION__<<__LINE__<<p.WatchDogItems.count();
    for(auto item:p.WatchDogItems)
    {
        SimpleWatchDogItem *tmpItem = CreateItem("SimpleWatchDogItem");
        tmpItem->SetName(item.Name);
        tmpItem->SetConfig(item.APPPath,p.CycleInterval);
        _watchDogItems.append(tmpItem);
        qDebug()<<__FUNCTION__<<__LINE__<<item.Name<<"初始化完成";
    }
    AppUtils::AutoRunWithSystem(p.IsAutoRun);
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
    QList<QString> ret;
    for(auto item:_watchDogItems)
    {
        ret.append(item->Name());
    }
    return ret;
}
