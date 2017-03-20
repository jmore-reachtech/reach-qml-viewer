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

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include "connection.h"
#include "messagehandler.h"
#include "settings.h"
#include "screen.h"
#include "watchdog.h"
#include "beep.h"

class MainView : public QQuickView
{
    Q_OBJECT
    
public:
    explicit MainView(QWindow *parent = 0);
    ~MainView();

public slots:
    void onMessageAvailable(const QString &item, const QString &property, const QVariant &value);
    void onMessageSyntaxError(const QByteArray &msg);
    void enableLookupAck(bool enable);
    // Qt signal handler.
    void handleSigTerm();

private slots:
    void onConnectionReady();
    void onConnectionClosed();

private:
    Connection *m_connection;
    MessageHandler *m_messageHandler;
    Settings *m_settings;
    Screen *m_screen;
    Watchdog *m_watchdog;
    Beep *m_beep;
    bool m_enableAck;
};

#endif // MAINVIEW_H
