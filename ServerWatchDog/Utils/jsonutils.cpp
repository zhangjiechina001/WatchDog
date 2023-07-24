#include "jsonutils.h"
#include <QDebug>
#include <QDir>

JsonUtils::JsonUtils(QObject *parent) : QObject(parent)
{}

bool JsonUtils::ReadJsonFile(const QString &fn, QJsonObject &jsonObj)
{
    QFile jsonFile(fn);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QByteArray jsonData = jsonFile.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    jsonObj=jsonDoc.object();
    jsonFile.close();
    return true;
}

bool JsonUtils::LoadJsonObject(const QString title, QJsonObject &jsonObj,bool updateFromJsonobj)
{
    QDir dir("./Config");
    if(!dir.exists())
    {
        dir.mkdir("./Config");
        qDebug()<<"Create ./Config dir";
    }

    QString fn="./Config/"+title+".json";
    TryCreateFile(jsonObj,fn);

    QFile jsonFile(fn);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray jsonData = jsonFile.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    QJsonObject loadObj=jsonDoc.object();
    jsonFile.close();

    //如果需要从传入jsonObj更新配置
    if(updateFromJsonobj)
    {
        bool needUpdate=false;
        for(auto key:jsonObj.keys())
        {
            if(loadObj.contains(key))
            {
                jsonObj[key]=loadObj[key];
            }
            else
            {
                loadObj[key]=jsonObj[key];
                needUpdate=true;
            }
        }

        if(needUpdate)
        {
            WriteJsonObject(fn,loadObj);
        }
    }
    else
    {
        jsonObj=loadObj;
    }

    return true;
}

bool JsonUtils::WriteJsonObject(const QString &fn, const QJsonObject &jsonObj)
{
    QFile jsonFile(fn);
    jsonFile.open(QIODevice::WriteOnly);
    // 创建Json文档
    QJsonDocument doc(jsonObj);
    jsonFile.write(doc.toJson());
    jsonFile.close();
    return true;
}

bool JsonUtils::LoadJsonIni(QString group, QMap<QString, QVariant> &map)
{
    QString fn="./controlSet.ini";
    QSettings* temp=new QSettings(fn,QSettings::IniFormat);
    //        temp->setIniCodec(QTextCodec::codecForName("utf-8"));
    if(!temp->childGroups().contains(group))
    {
        temp->beginGroup(group);
        for(QString key:map.keys())
        {
            temp->setValue(key,map[key]);
        }
        temp->sync();
        temp->endGroup();
    }

    temp->beginGroup(group);
    for(QString key:map.keys())
    {
        if(temp->contains(key))
        {
            map[key]=temp->value(key);
        }
        else
        {
            temp->setValue(key,map[key]);
            temp->sync();
        }
    }
    temp->endGroup();
    delete temp;
    return true;
}

bool JsonUtils::SaveJsonIni(QString group, QMap<QString, QVariant> &map)
{
    QString fn="./controlSet.ini";
    QSettings* temp=new QSettings(fn,QSettings::IniFormat);
    //        temp->setIniCodec(QTextCodec::codecForName("utf-8"));
    temp->beginGroup(group);
    for(QString key:map.keys())
    {
        temp->setValue(key,map[key]);
        temp->sync();
    }
    temp->endGroup();
    delete temp;
    return true;
}

bool JsonUtils::TryCreateFile(const QJsonObject &jsonObj, QString fn)
{
    QFile jsonFile(fn);

    if(!jsonFile.exists())
    {
        // 创建Json文档
        jsonFile.open(QIODevice::WriteOnly);
        QJsonDocument doc(jsonObj);
        jsonFile.write(doc.toJson());
        jsonFile.close();
    }
    return true;
}
