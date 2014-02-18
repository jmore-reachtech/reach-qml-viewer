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

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QDeclarativeContext>

#include "connection.h"
#include "messagehandler.h"

class MainView : public QDeclarativeView
{
    Q_OBJECT
    
public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();
    bool HeartbeatEnabled();

public slots:
    void onMessageAvailable(const QString &item, const QString &property, const QVariant &value);
    void onMessageSyntaxError(const QByteArray &msg);
    void enableLookupAck(bool enable);
    void enableHeartbeat(bool enable);

private slots:
    void onConnectionReady();
    void onConnectionClosed();
    void onHeartbeatTimerTimeout();

private:
    Connection *m_connection;
    MessageHandler *m_messageHandler;
    bool m_enableAck;
    bool m_enableHeartbeat;
    QTimer *m_heartbeatTimer;
};

#endif // MAINVIEW_H
