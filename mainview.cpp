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

#include "mainview.h"

MainView::MainView(QWidget *parent) :
    QDeclarativeView(parent)
  ,m_connection(new Connection(this))
  ,m_messageHandler(new MessageHandler)
  ,m_settings(new Settings(this))
  ,m_screen(new Screen(this))
  ,m_enableAck(false)
{
    this->rootContext()->setContextProperty("connection", m_connection);
    this->rootContext()->setContextProperty("settings", m_settings);
    this->rootContext()->setContextProperty("screen", m_screen);

    connect(m_connection,SIGNAL(readyToSend()),this,SLOT(onConnectionReady()));
    connect(m_connection,SIGNAL(notReadyToSend()),this,SLOT(onConnectionClosed()));
    connect(m_connection,SIGNAL(messageAvailable(QByteArray)),m_messageHandler,SLOT(onMessageAvailable(QByteArray)));
    connect(m_connection,SIGNAL(lookupAckChanged(bool)),this,SLOT(enableLookupAck(bool)));
    connect(m_messageHandler,SIGNAL(messageAvailable(QString,QString,QVariant)),this,SLOT(onMessageAvailable(QString,QString,QVariant)));
    connect(m_messageHandler,SIGNAL(messageSyntaxError(QByteArray)),this,SLOT(onMessageSyntaxError(QByteArray)));
}

MainView::~MainView()
{
}


void MainView::onMessageAvailable(const QString &item, const QString &property, const QVariant &value)
{
    QDeclarativeItem *obj = this->rootObject()->findChild<QDeclarativeItem*>(item);
    if(!obj) {
        qDebug() << "[QML] no item with objectName: " << item;
        if(m_enableAck) {
            m_connection->sendMessage("LUNO");
        }
        return;
    }

    bool found = obj->setProperty(property.toLatin1(),value);
    if (!found) {
        qDebug() << "[QML] no property on objectName: " << property;
        if(m_enableAck) {
            m_connection->sendMessage("LUNP");
        }
        return;
    }

    if(m_enableAck) {
        m_connection->sendMessage("LUOK");
    }
}

void MainView::onMessageSyntaxError(const QByteArray &msg)
{
    if(m_enableAck) {
        m_connection->sendMessage("SYNERR");
    }
}

void MainView::enableLookupAck(bool enable)
{
    m_enableAck = enable;
    if (m_enableAck)
    {
        qDebug() << "[QML] Ack enabled";
    }
    else
    {
        qDebug() << "[QML] Ack disabled";
    }
}

void MainView::onConnectionReady()
{
    qDebug() << "[QML] connection open";
}

void MainView::onConnectionClosed()
{
    qDebug() << "[QML] connection closed";
}


