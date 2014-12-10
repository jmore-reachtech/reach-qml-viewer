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
  ,m_heartbeatText(HEARTBEAT_TEXT)
  ,m_heartbeatResponseText(HEARTBEAT_RESPONSE_TEXT)
  ,m_heartbeat_interval(0)
  ,m_hearbeatTimer(new QTimer(this))
{
    connect(m_socket,SIGNAL(connected()),this,SLOT(onSocketConnected()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(onSocketDisconnected()));
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
    connect(m_socket,SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),this,SLOT(onSocketStateChange(QLocalSocket::LocalSocketState)));
    connect(m_connectTimer, SIGNAL(timeout()),this,SLOT(onConnectTimerTimeout()));
    connect(m_hearbeatTimer,SIGNAL(timeout()),this,SLOT(onHeartbeatTimerTimeout()));

    m_connectTimer->start(5000);
}

Connection::~Connection()
{
    qDebug() << "[QML] connection destructor";
    if(m_socket->isOpen()) {
        qDebug() << "[QML] closing socket";
        m_socket->close();
    }
    delete m_socket;
    delete m_connectTimer;
}

void Connection::onSocketConnected()
{
    qDebug() << "[QML] socket connected";
    m_connectTimer->stop();

    emit readyToSend();
}

void Connection::onSocketDisconnected()
{
    qDebug() << "[QML] socket disconnected";
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
             qDebug() << "[QML] QLocalSocket::ServerNotFoundError";
             break;
         case QLocalSocket::ConnectionRefusedError:
             qDebug() << "[QML] QLocalSocket::ConnectionRefusedError";
             break;
         case QLocalSocket::PeerClosedError:
             qDebug() << "[QML] QLocalSocket::PeerClosedError";
             break;
         default:
            qDebug() << m_socket->errorString();
            break;
    }
}

void Connection::onSocketReadyRead()
{
    //qDebug() << "[QML] data available on socket";
    while (m_socket->canReadLine()) {
        qDebug() << "[QML] message available on socket";
        QByteArray ba = m_socket->readLine();

        /* check for hearbeat - must be a pong */
        if(ba.trimmed() == m_heartbeatResponseText) {
            qDebug() << "[QML] got " << m_heartbeatResponseText;
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
            qDebug() << "[QML] socket state UnconnectedState";
            break;
        case QLocalSocket::ConnectingState:
            qDebug() << "[QML] socket state ConnectingState";
            break;
        case QLocalSocket::ConnectedState:
            qDebug() << "[QML] socket state ConnectedState";
            break;
        case QLocalSocket::ClosingState:
            qDebug() << "[QML] socket state ClosingState";
            break;
        default:
            qDebug() << "[QML] unknown state";
            break;
    }
}

void Connection::sendMessage(const QString &message)
{
    //TODO: revisit
    QString msg(message);
    int sent = 0;

    qDebug() << "[QML] sending => " << msg;
    sent = m_socket->write(msg.append(MESSAGE_TERMINATOR).toLatin1());
    if(sent == -1) {
        qDebug() << "[QML] socket->write() error" << __FUNCTION__;
    }

    if (m_socket->bytesToWrite() > 0) {
        if(m_socket->flush()) {
            qDebug() << "[QML] socket flushed";
        }
    }
}

void Connection::updateValue(const QString &objectName, const QString &property, const QVariant &value)
{
    sendMessage(objectName.toLatin1() + "." + property.toLatin1() + "=" + value.toByteArray());
}

void Connection::enableHeartbeat(int interval)
{
    qDebug() << "[QML] hearbeat enabled ";
    m_heartbeat_interval = interval;
    m_hearbeatTimer->stop();
    m_hearbeatTimer->start((m_heartbeat_interval * 1000));
}

void Connection::enableHeartbeat(int interval, QString heartbeatText, QString heartbeatResponseText)
{
    qDebug() << "[QML] hearbeat enabled ";
    m_heartbeatText = heartbeatText;
    m_heartbeatResponseText = heartbeatResponseText;
    m_heartbeat_interval = interval;
    m_hearbeatTimer->stop();
    m_hearbeatTimer->start((m_heartbeat_interval * 1000));
}

void Connection::disableHeartbeat()
{
    qDebug() << "[QML] hearbeat disabled ";
    if(m_hearbeatTimer->isActive()) {
        m_hearbeatTimer->stop();
    }
}

void Connection::disableLookupAck()
{
    m_enableAck = false;
    emit lookupAckChanged(false);
}

void Connection::enableLookupAck()
{
    m_enableAck = true;
    emit lookupAckChanged(true);
}

void Connection::tryConnect()
{
    bool startHeartbeat = false;
    m_connectTimer->stop();

    QSettings settings(SYSTEM_SETTINGS_FILE,QSettings::NativeFormat);
    settings.beginGroup(SYSTEM_SETTINGS_SECTION);

    m_socket->connectToServer(settings.value("socket_path","/tmp/tioAgent").toString());
    m_heartbeat_interval = settings.value("hearbeat_interval",0).toInt();
    startHeartbeat = settings.value("enable_heartbeat",false).toBool();

    settings.endGroup();
    if (!m_socket->waitForConnected(1000)) {
        qDebug() << "[QML] could not connect: setting retry timer";
        m_connectTimer->start(5000);
    } else {
        qDebug() << "[QML] conection isValid" << m_socket->isValid();
        qDebug() << "[QML] conection bytesAvailable" << m_socket->bytesAvailable();
        qDebug() << "[QML] conection isReadable" << m_socket->isReadable();
    }

    /* set up heartbeat if need be */
    if (startHeartbeat)
    {
        m_hearbeat = false;
        if(m_heartbeat_interval > 0) {
            enableHeartbeat(m_heartbeat_interval);
        }
        else
        {
            /* interval was not provided - default to 5 seconds */
            enableHeartbeat(5);
        }
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
    sendMessage(m_heartbeatText);
}

