#include "fileutils.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

FileUtils::FileUtils()
{

}

QString FileUtils::CreateDirFromDatetime(QString baseFloder, QDateTime dt)
{
    QString folder=baseFloder+"//"+dt.toString("yyyy-MM-dd_hh-mm-ss");
    QDir dir;
    if(dir.mkdir(folder))
    {
        return folder;
    }
    else
    {
        return "";
    }
}

bool FileUtils::ExportToCsv(QMap<QString, QList<double> > data, QString fileName)
{
    if(data.keys().count()==0||(data.keys().count()!=0&&data.first().count()==0))
    {
        qDebug()<<"data is empty!";
        return false;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open file fail";
        return false;
    }

    QTextStream out(&file);

    int col=data.count();
    int row=data.first().count();
    //写表头
    for(int i = 0;i < col; i++)
    {
        out<<data.keys().at(i);
        QString end=(i==(col-1))?"\n":",";;
        out<<end;
    }
    //写数据
    for(int i = 0;i < row; i++)
    {
        for(int j = 0;j < col; j++)
        {
            data[data.keys().at(j)].at(i);
            out<<data[data.keys().at(j)].at(i);
            QString end=(j==(col-1))?"\n":",";;
            out<<end;
        }
    }
    file.close();

    return true;
}

bool FileUtils::ExportToCsv(QList<QList<QString> > data, QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open file fail";
        return false;
    }

    QTextStream out(&file);

    //写数据
    int row=data.count();
    int col=data.first().count();
    for(int i = 0;i < row; i++)
    {
        for(int j = 0;j < col; j++)
        {
            out<<data.at(i).at(j);
            QString end=(j==(col-1))?"\n":",";;
            out<<end;
        }
    }
    file.close();

    return true;
}
