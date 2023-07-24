#ifndef TABLEWIDGETUTILS_H
#define TABLEWIDGETUTILS_H

#include <QMap>
#include <QList>
#include <QTableWidget>
using StringMat=QList<QList<QString>>;

class TableWidgetUtils
{
public:
    TableWidgetUtils();

    static QMap<QString, QList<double>> GetTableDataByColumn(const QTableWidget* tableWidget,int startColumnIndex);
    static QMap<QString, QList<double>> GetTableDataByColumn(const QTableWidget* tableWidget,QList<int> notIncludeIndex);
    static QMap<QString, QList<double>> GetTableDataByColumnImpl(const QTableWidget* tableWidget,QList<int> columnIndex);

    static StringMat GetMat(const QTableWidget* tableWidget,QList<int> notIncludeIndex);
};

#endif // TABLEWIDGETUTILS_H
