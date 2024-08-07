#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include "Head.h"
#include <QObject>
#include <QSettings>

struct WatchDogConfigItem
{
    WatchDogConfigItem()
    {
        Name="";
        APPPath="";
    }
    QString Name;
    QString APPPath;
};

struct WatchDogParam
{
    WatchDogParam()
    {
        IsAutoRun=false;
        CycleInterval=10;
    }

    bool IsAutoRun;

    int CycleInterval;

    QList<WatchDogConfigItem> WatchDogItems;
};

class GlobalVar
{
private:
    GlobalVar();
    GlobalVar(const GlobalVar&)=delete;
    GlobalVar& operator=(const GlobalVar&)=delete;

public:
    static GlobalVar &Instance();

    QSettings* Setting;

    WatchDogParam GetWatchDogParam();
    void SetWatchDogParam(WatchDogParam p);
};

#endif // GLOBALVAR_H
