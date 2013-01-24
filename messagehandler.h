#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QDebug>
#include <QVariant>

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageHandler(QObject *parent = 0);
    
signals:
    void messageAvailable(const QString &item, const QString &property, const QVariant &value);
    void messageSyntaxError(const QByteArray &msg);

public slots:
    void onMessageAvailable(QByteArray);
    
};

#endif // MESSAGEHANDLER_H
