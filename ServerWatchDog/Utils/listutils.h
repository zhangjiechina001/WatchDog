#ifndef LISTUTILS_H
#define LISTUTILS_H
#include <QList>

class ListUtils
{
public:
    ListUtils();

    static QList<QString> Intersection(QList<QString> list1,QList<QString> list2);

    static QList<double> SplitArray(double n, double step);
};

#endif // LISTUTILS_H
