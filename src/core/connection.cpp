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

#include "connection.h"

Connection::Connection(QObject *parent) :
    QObject(parent)
  ,m_socket(new QLocalSocket(this))
  ,m_connectTimer(new QTimer(this))
  ,m_enableAck(false)
  ,m_hearbeat(true)
  ,m_heartbeat_interval(0)
  ,m_hearbeatTimer(new QTimer(this))
{
    connect(m_socket,SIGNAL(connected()),this,SLOT(onSocketConnected()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(onSocketDisconnected()));
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
    connect(m_socket,SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),this,SLOT(onSocketStateChange(QLocalSocket::LocalSocketState)));
    connect(m_connectTimer, SIGNAL(timeout()),this,SLOT(onConnectTimerTimeout()));
    connect(m_hearbeatTimer,SIGNAL(timeout()),this,SLOT(onHeartbeatTimerTimeout()));

    m_connectTimer->start(1000);
}

Connection::~Connection()
{
    qDebug() << "connection destructor";
    if(m_socket->isOpen()) {
        qDebug() << "closing socket";
        m_socket->close();
    }
    delete m_socket;
    delete m_connectTimer;
}

void Connection::onSocketConnected()
{
    qDebug() << "socket connected";
    m_connectTimer->stop();

    emit readyToSend();
}

void Connection::onSocketDisconnected()
{
    qDebug() << "socket disconnected";
    m_connectTimer->start(5000);

    emit notReadyToSend();

    /* stop the heartbeat while we wait for a connection */
    if(m_hearbeatTimer->isActive()) {
        m_hearbeatTimer->stop();
        emit noHeartbeat();
    }
}

void Connection::onSocketError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError) {
         case QLocalSocket::ServerNotFoundError:
             qDebug() << "QLocalSocket::ServerNotFoundError";
             break;
         case QLocalSocket::ConnectionRefusedError:
             qDebug() << "QLocalSocket::ConnectionRefusedError";
             break;
         case QLocalSocket::PeerClosedError:
             qDebug() << "QLocalSocket::PeerClosedError";
             break;
         default:
            qDebug() << m_socket->errorString();
            break;
    }
}

void Connection::onSocketReadyRead()
{
    qDebug() << "data available on socket";
    while (m_socket->canReadLine()) {
        qDebug() << "message available on socket";
        QByteArray ba = m_socket->readLine();

        /* check for hearbeat - must be a pong */
        if(ba.trimmed() == "pong") {
            qDebug() << "got pong";
            if(m_hearbeatTimer->isActive()) {
                m_hearbeat = true;
                emit heartbeat();
            }
            continue;
        } else {
            emit messageAvailable(ba);
        }
    }
}

void Connection::onSocketStateChange(QLocalSocket::LocalSocketState socketState)
{
    switch(socketState) {
        case QLocalSocket::UnconnectedState:
            qDebug() << "socket state UnconnectedState";
            break;
        case QLocalSocket::ConnectingState:
            qDebug() << "socket state ConnectingState";
            break;
        case QLocalSocket::ConnectedState:
            qDebug() << "socket state ConnectedState";
            break;
        case QLocalSocket::ClosingState:
            qDebug() << "socket state ClosingState";
            break;
        default:
            qDebug() << "unknown state";
            break;
    }
}

void Connection::sendMessage(const QString &message)
{
    //TODO: revisit
    QString msg(message);
    int sent = 0;

    qDebug() << "sending message: " << msg;
    sent = m_socket->write(msg.append("\n").toLatin1());
    if(sent == -1) {
        qDebug() << "socket->write() error" << __FUNCTION__;
    }

    if (m_socket->bytesToWrite() > 0) {
        if(m_socket->flush()) {
            qDebug() << "socket flushed";
        }
    }
}

void Connection::updateValue(const QString &objectName, const QString &property, const QVariant &value)
{
    sendMessage(objectName.toLatin1() + "." + property.toLatin1() + "=" + value.toByteArray());
}

void Connection::enableHeartbeat(int interval)
{
    qDebug() << "enabling hearbeat ";

    m_heartbeat_interval = interval;

    if(!m_hearbeatTimer->isActive()) {
        m_hearbeatTimer->stop();
        m_hearbeatTimer->start((m_heartbeat_interval * 1000));
    }
}

void Connection::disableHeartbeat()
{
    qDebug() << "disabling hearbeat ";

    if(m_hearbeatTimer->isActive()) {
        m_hearbeatTimer->stop();
    }
}

void Connection::tryConnect()
{
    m_connectTimer->stop();

    QSettings settings("$HOME/setting.conf",QSettings::NativeFormat);
    settings.beginGroup("system");

    m_socket->connectToServer(settings.value("socket_path","/tmp/tioAgent").toString());
    m_enableAck = settings.value("enable_ack",false).toBool();
    m_heartbeat_interval = settings.value("hearbeat_interval",0).toInt();
    settings.endGroup();
    if (!m_socket->waitForConnected(1000)) {
        qDebug() << "could not connect: setting retry timer";
        m_connectTimer->start(5000);
    } else {
        qDebug() << "conection isValid" << m_socket->isValid();
        qDebug() << "conection bytesAvailable" << m_socket->bytesAvailable();
        qDebug() << "conection isReadable" << m_socket->isReadable();
    }

    /* set up heartbeat timer if set */
    if(m_heartbeat_interval > 0) {
        m_hearbeat = false;
        m_hearbeatTimer->start((m_heartbeat_interval * 1000));
    }
}

void Connection::onConnectTimerTimeout()
{
    tryConnect();
}

void Connection::onHeartbeatTimerTimeout()
{
    /* if we have not received a pong emit signal*/
    if(!m_hearbeat) {
        emit noHeartbeat();
    }
    m_hearbeat = false;
    sendMessage("ping");
}

