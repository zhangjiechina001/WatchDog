#include "listutils.h"

#include <QSet>
ListUtils::ListUtils()
{

}

QList<QString> ListUtils::Intersection(QList<QString> list1, QList<QString> list2)
{
    // 将list1和list2转换为QSet
    QSet<QString> set1 = QSet<QString>::fromList(list1);
    QSet<QString> set2 = QSet<QString>::fromList(list2);
    return set1.intersect(set2).toList();
}

QList<double> ListUtils::SplitArray(double n, double step)
{
    QList<double> ret;
    double current=qAbs(n);
    while(current>0)
    {
        double chunk=qMin(current,step);
        ret.append(n>0?chunk:-chunk);
        current-=chunk;
    }
    return ret;
}
