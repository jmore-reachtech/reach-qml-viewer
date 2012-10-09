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
    void onItemAvailable(const QString &item, const QString &property, const QVariant &value);
    void enableLookupAck(bool enable);
    void enableHeartbeat(bool enable);

signals:

private slots:
    void onConnectionOpen();
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
