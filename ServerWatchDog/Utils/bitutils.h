#ifndef BITUTILS_H
#define BITUTILS_H

#include <QList>

class BitUtils
{
public:
    BitUtils();

    static QList<uint16_t> ConvertToUint16List(int data);

    static int ConvertToInt(QList<uint16_t> data);

    static QString HexToString(QByteArray data);

};

#endif // BITUTILS_H
