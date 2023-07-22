#include "license.h"
#include "computer.h"
#include "b64.h"
extern "C" {
#include "rsa.h"
}
#include <QDebug>

static struct key_class priv_key;

bool checkLicenseFile(QFile &file)
{
    if (file.exists()) {
       if (file.open(QFile::ReadOnly)) {
           QByteArray data = file.readAll();
           file.close();
           return decodeLiceseData(data);
       }
    }
    return false;
}

bool decodeLiceseData(QByteArray &data)
{
    priv_key.modulus = 107339917;
    priv_key.exponent = 28813325;

    int len = data.size();

    // 1 首先B64解码
    uchar* decstr = b64_decode(data.data(), len);
    if (!decstr) {
        return false;
    }
    char serial[256];
    int rc = readPhysicalDrive(serial, NULL);
    if (rc == 0) {
        return false;
    }
    QString serailAayy = QString(serial).trimmed();
    len = serailAayy.size();
    qDebug() << "\nstart decodeLiceseData len:" << len;
    // 2. Rsa解码
//    QByteArray decData;
//    decData.resize((len + 1) * sizeof(long long));
//    memcpy(decData.data(), decstr, (len + 1) * sizeof(long long));
    long long *entydata = (long long*)decstr;
    char *decrypted = rsa_decrypt(entydata, 8*(len + 1 ), &priv_key);
    if (!decrypted){
       free(decstr);
       return false;
    }
    QString decodeA(decrypted);
    qDebug() << "\nstart decodeLiceseData decrypted:" << QString(decrypted).trimmed();
    bool ret = false;
    if (serailAayy == decodeA) {
        ret  = true;
    }
    free(decstr);
    free(decrypted);
    return ret;
}
