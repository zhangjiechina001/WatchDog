#pragma execution_character_set("utf-8")
#ifndef JOSNUTILS_H
#define JOSNUTILS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QSettings>
#include <QTextCodec>

class JsonUtils : public QObject
{
    Q_OBJECT

public:
    explicit JsonUtils(QObject *parent = nullptr);

    static bool ReadJsonFile(const QString &fn,QJsonObject &jsonObj);

    static bool LoadJsonObject(const QString title,QJsonObject &jsonObj,bool updateFromJsonobj=false);

    static bool WriteJsonObject(const QString &fn,const QJsonObject &jsonObj);

    static bool LoadJsonIni(QString group,QMap<QString,QVariant>& map);

    static bool SaveJsonIni(QString group,QMap<QString,QVariant>& map);

private:
    static bool TryCreateFile(const QJsonObject &jsonObj, QString fn);

signals:

public slots:
};


#endif // JOSNUTILS_H
