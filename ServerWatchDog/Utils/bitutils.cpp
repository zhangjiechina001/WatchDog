#include "bitutils.h"

BitUtils::BitUtils()
{

}

QList<uint16_t> BitUtils::ConvertToUint16List(int data)
{
    QList<uint16_t> ret;
    ret.append(data >> 16);//高位
    ret.append(data & 0xFFFF);//低位
    return ret;
}

int BitUtils::ConvertToInt(QList<uint16_t> data)
{
    int data0=data.at(0);
    return (data0 << 16) + data.at(1);
}

QString BitUtils::HexToString(QByteArray data)
{

    QString hexString = data.toHex();
    QString formattedString;
    for (int i = 0; i < hexString.length(); i += 2) {
        formattedString +=  hexString.mid(i, 2) + " ";
    }
    return formattedString;

}
