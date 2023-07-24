#include "tablewidgetutils.h"

TableWidgetUtils::TableWidgetUtils()
{

}

QMap<QString, QList<double> > TableWidgetUtils::GetTableDataByColumn(const QTableWidget *tableWidget, int startColumnIndex)
{
    QList<int> columnIndex;
    for(int i=startColumnIndex;i<tableWidget->columnCount();i++)
    {
        columnIndex.append(i);
    }
    return GetTableDataByColumnImpl(tableWidget,columnIndex);
}

QMap<QString, QList<double> > TableWidgetUtils::GetTableDataByColumn(const QTableWidget *tableWidget, QList<int> notIncludeIndex)
{
    QList<int> columnIndex;
    for(int i=0;i<tableWidget->columnCount();i++)
    {
        if(!notIncludeIndex.contains(i))
        {
            columnIndex.append(i);
        }
    }
    return GetTableDataByColumnImpl(tableWidget,columnIndex);
}

QMap<QString, QList<double> > TableWidgetUtils::GetTableDataByColumnImpl(const QTableWidget *tableWidget, QList<int> columnIndex)
{
    QMap<QString, QList<double>> dataMap;

    int rowCount = tableWidget->rowCount();

    for(int column:columnIndex)
    {
        QString columnName = tableWidget->horizontalHeaderItem(column)->text();
        QList<double> columnData;

        for (int row = 0; row < rowCount; ++row) {
            QTableWidgetItem* item = tableWidget->item(row, column);
            if (item != nullptr) {
                bool ok;
                double value = item->text().toDouble(&ok);
                if (ok) {
                    columnData.append(value);
                }
            }
        }
        dataMap.insert(columnName, columnData);
    }
    return dataMap;
}

StringMat TableWidgetUtils::GetMat(const QTableWidget *tableWidget, QList<int> notIncludeIndex)
{
    // 创建一个QList<QList<QString>>来存储转换后的数据
    QList<QList<QString>> dataList;

    QList<QString> headerData;
    for (int col = 0; col < tableWidget->columnCount(); ++col)
    {
        if(!notIncludeIndex.contains(col))
        {
            headerData.append(tableWidget->horizontalHeaderItem(col)->text());
        }
    }
    dataList.append(headerData);

    // 遍历QTableWidget的行和列，并提取数据到dataList中
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QList<QString> rowData;
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            if(!notIncludeIndex.contains(col))
            {
                QTableWidgetItem *item = tableWidget->item(row, col);
                if (item) {
                    rowData.append(item->text());
                } else {
                    rowData.append(""); // 添加空字符串，以保持数据对齐
                }
            }
        }
        dataList.append(rowData);
    }
    return dataList;
}
