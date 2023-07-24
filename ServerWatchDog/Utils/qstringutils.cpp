#include "qstringutils.h"

QStringUtils::QStringUtils(QObject *parent) : QObject(parent)
{

}

QString QStringUtils::MapToString(QMap<QString, double> map)
{
    QString ret;
    for(auto key:map.keys())
    {
        ret.append(QString("%1:%2 ").arg(key).arg(map[key]));
    }
    return ret;
}
