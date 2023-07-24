#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QString>
#include <QDateTime>

class FileUtils
{
public:
    FileUtils();

    static QString CreateDirFromDatetime(QString baseFloder,QDateTime dt);

    static bool ExportToCsv(QMap<QString,QList<double>> data,QString fileName);

    static bool ExportToCsv(QList<QList<QString>> data,QString fileName);
};

#endif // FILEUTILS_H
