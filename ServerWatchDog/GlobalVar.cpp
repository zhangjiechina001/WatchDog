#include "GlobalVar.h"

GlobalVar::GlobalVar()
{
    QString initFile=QString("./Config/%1").arg("ClientConfig.ini");
    Setting=new QSettings(initFile,QSettings::IniFormat);
    Setting->setValue("sample","-----");
    Setting->sync();
}

GlobalVar &GlobalVar::Instance()
{
    static GlobalVar _instance;
    return _instance;
}

WatchDogParam GlobalVar::GetWatchDogParam()
{
    QSettings *set=Setting;
    if(!set->contains("WatchDogParam/IsAutoRun"))
    {
        WatchDogParam setParam;
        SetWatchDogParam(setParam);
    }

    WatchDogParam ret;
    set->beginGroup("WatchDogParam");
    ret.IsAutoRun=Setting->value("IsAutoRun").toBool();
    ret.CycleInterval=Setting->value("CycleInterval",5).toInt();

    if(!Setting->value("APPNames").toString().isEmpty())
    {
        QStringList names=Setting->value("APPNames").toString().split("|");
        QStringList paths=Setting->value("APPPaths").toString().split("|");
        for(int i=0;i<names.count();i++)
        {
            WatchDogConfigItem item;
            item.Name=names.at(i);
            item.APPPath=paths.at(i);
            ret.WatchDogItems.append(item);
        }
    }

    set->endGroup();
    return ret;
}

void GlobalVar::SetWatchDogParam(WatchDogParam p)
{
    QSettings *set=Setting;
    set->beginGroup("WatchDogParam");
    set->setValue("IsAutoRun",p.IsAutoRun);
    set->setValue("CycleInterval",p.CycleInterval);

    QStringList names;
    QStringList paths;
    for(auto item:p.WatchDogItems)
    {
        names.append(item.Name);
        paths.append(item.APPPath);
    }
    set->setValue("APPNames",names.join("|"));
    set->setValue("APPPaths",paths.join("|"));
    set->endGroup();
    set->sync();
}
