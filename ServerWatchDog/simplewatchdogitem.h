#ifndef SIMPLEWAITCHDOGITEM_H
#define SIMPLEWAITCHDOGITEM_H

#include <QObject>
#include <QTimer>
#include <QProcess>
#include <QJsonObject>

class SimpleWatchDogItem : public QObject
{
    Q_OBJECT
public:
    enum Status
    {
        Off = 0,
        Block = 1,
        Running=2,
    };
    Q_ENUM(Status)

    explicit SimpleWatchDogItem(QObject *parent = nullptr);
    virtual void WaitForEnd();

    void SetName(QString name);
    QString Name();

    virtual void SetConfig(QJsonObject obj);

    void InitProcess(QString filePath);

    bool StartProgram();
signals:
    void StatusChanged(Status currentStatus);

public slots:
    virtual void CheckStatus();

private:
    QProcess  m_process;

    QString _name;
    QTimer _timer;
};

#endif // SIMPLEWAITCHDOGITEM_H
