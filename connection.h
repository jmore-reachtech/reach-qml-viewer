/*************************************************************************
**
**    (C) Copyright 2013 Reach Technology Inc.
**
**    This code is protected by international copyright laws. This file may
**    only be used in accordance with a license and cannot be used on
**    hardware other than supplied by Reach Technology Inc. We appreciate
**    your understanding and fairness.
**
*************************************************************************/

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
    void enableHeartbeat(int);
    void disableHeartbeat();

signals:
    void messageAvailable(const QByteArray &message);
    void readyToSend();
    void notReadyToSend();
    void noHeartbeat();
    void heartbeat();

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QLocalSocket::LocalSocketError);
    void onSocketReadyRead();
    void onSocketStateChange(QLocalSocket::LocalSocketState );
    void tryConnect();
    void onConnectTimerTimeout();
    void onHeartbeatTimerTimeout();

private:
    QLocalSocket *m_socket;
    QTimer *m_connectTimer;
    bool    m_enableAck;
    bool    m_hearbeat;
    int     m_heartbeat_interval;
    QTimer  *m_hearbeatTimer;
};

#endif // CONNECTION_H

