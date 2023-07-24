#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QDir>
#include <QDate>


static QMutex _logMutex;

class Logger : public QObject
{
    Q_OBJECT
private:
    Logger()
    {

    }
    Logger(const Logger&)=delete;
    Logger& operator =(const Logger&)=delete;

public:
    static Logger& Instance()
    {
        static Logger _instance;
        return _instance;
    }

signals:
    void OnLogging(QString log);
    void OnLogging(QtMsgType type,const QMessageLogContext &context,const QString &msg);

public slots:


public:

    static int myInt;
    static void CustomMessageHandler(QtMsgType type,const QMessageLogContext &context,const QString &msg)
    {
        QDateTime _datetime = QDateTime::currentDateTime();
        QString szDate = _datetime.toString("yyyy-MM-dd hh:mm:ss.zzz");
        QString txt(szDate);

        QMap<QtMsgType,QString> msgTypeMap=
        {
            {QtDebugMsg,"[Debug]"},
            {QtCriticalMsg,"[Critical]"},
            {QtInfoMsg,"[Info]"},
            {QtWarningMsg,"[Warning]"},
            {QtFatalMsg,"[Fatal"},
        };

        txt+=QString(" %1 ").arg(msgTypeMap[type]);
        txt.append(msg);

        _logMutex.lock();
        QDir dir("./");
        if (!dir.exists("log")) {
            dir.mkpath("log");
        }
        QString fn =QString("./log/%1%2_log.txt").arg(msgTypeMap[type],QDate::currentDate().toString("yyyy_MM_dd"));
        if(QtDebugMsg==type)
        {
            emit Logger::Instance().OnLogging(msg);
            emit Logger::Instance().OnLogging(type,context,msg);
        }
        QFile file(fn);
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << txt << "\r\n";
        file.close();
        _logMutex.unlock();
    }
};


#endif // LOGGER_H
