#ifndef LICENSE_H
#define LICENSE_H
#include <QFile>

#ifdef __cplusplus
extern "C" {
#endif
bool checkLicenseFile(QFile &file);
bool decodeLiceseData(QByteArray &data);
#ifdef __cplusplus
}
#endif
#endif // LICENSE_H
