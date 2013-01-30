#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>

#include "systemdefs.h"

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);
    ~Connection();
    
signals:
    
public slots:
    void sendMessage(const QString &message);
    void updateValue(const QString &objectName, const QString &property, const QVariant &value);

signals:
    void connectionOpen();
    void connectionClosed();
    void messageAvailable(const QByteArray &message);
    void connectionReady();

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QLocalSocket::LocalSocketError);
    void onSocketReadyRead();
    void onSocketStateChange(QLocalSocket::LocalSocketState );
    void tryConnect();
    void onConnectTimerTimeout();

private:
    QLocalSocket *m_socket;
    QTimer *m_connectTimer;
    bool m_enableAck;
};

#endif // CONNECTION_H
