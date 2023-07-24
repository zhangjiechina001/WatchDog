#ifndef QSTRINGUTILS_H
#define QSTRINGUTILS_H

#include <QObject>
#include <QMap>

class QStringUtils : public QObject
{
    Q_OBJECT
public:
    explicit QStringUtils(QObject *parent = nullptr);

    static QString MapToString(QMap<QString,double> map);
signals:

public slots:
};

#endif // QSTRINGUTILS_H
