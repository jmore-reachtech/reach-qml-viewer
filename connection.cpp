#include "connection.h"

Connection::Connection(QObject *parent) :
    QObject(parent)
  ,m_socket(new QLocalSocket(this))
  ,m_connectTimer(new QTimer(this))
  ,m_enableAck(false)
{
    connect(m_socket,SIGNAL(connected()),this,SLOT(onSocketConnected()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(onSocketDisconnected()));
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
    connect(m_socket,SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),this,SLOT(onSocketStateChange(QLocalSocket::LocalSocketState)));
    connect(m_connectTimer, SIGNAL(timeout()), SLOT(onConnectTimerTimeout()));

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

    sendMessage("wakeup");

    emit connectionOpen();
}

void Connection::onSocketDisconnected()
{
    qDebug() << "socket disconnected";
    m_connectTimer->start(5000);

    emit connectionClosed();
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
             break;
         default:
            qDebug() << m_socket->errorString();
    }
}

void Connection::onSocketReadyRead()
{
    qDebug() << "data available on socket";
    while (m_socket->canReadLine()) {
        qDebug() << "message available on socket";
        QByteArray ba = m_socket->readLine();

        emit messageAvailable(ba);
    }
}

void Connection::onSocketStateChange(QLocalSocket::LocalSocketState socketState)
{
    switch(socketState) {
        case QLocalSocket::UnconnectedState:
            qDebug() << "socket state UnconnectedState"; break;
        case QLocalSocket::ConnectingState:
            qDebug() << "socket state ConnectingState"; break;
        case QLocalSocket::ConnectedState:
            qDebug() << "socket state ConnectedState"; break;
        case QLocalSocket::ClosingState:
            qDebug() << "socket state ClosingState"; break;
        default:
            qDebug() << "unknown state"; break;
    }
}

void Connection::sendMessage(const QString &message)
{
    //TODO: revisit
    QString msg(message);
    int sent = 0;

    qDebug() << "sending message: " << msg;
    sent = m_socket->write(msg.append(MESSAGE_TERMINATOR).toLatin1());
    if(sent == -1) {
        qDebug() << "socket->write() error" << __FUNCTION__;
    }

    if (m_socket->bytesToWrite() > 0) {
        if(m_socket->flush()) {
            qDebug() << "socket flushed";
        }
    }
}

void Connection::tryConnect()
{
    m_connectTimer->stop();

    QSettings settings(SYSTEM_SETTINGS_FILE,QSettings::NativeFormat);
    settings.beginGroup(SYSTEM_SETTINGS_SECTION);

    m_socket->connectToServer(settings.value("socket_path","/tmp/tioAgent").toString());
    m_enableAck = settings.value("enable_ack",false).toBool();
    settings.endGroup();
    if (!m_socket->waitForConnected(1000)) {
        qDebug() << "could not connect: setting retry timer";
        m_connectTimer->start(5000);
    } else {
        qDebug() << "conection isValid" << m_socket->isValid();
        qDebug() << "conection bytesAvailable" << m_socket->bytesAvailable();
        qDebug() << "conection isReadable" << m_socket->isReadable();
    }
}

void Connection::onConnectTimerTimeout()
{
    tryConnect();
}

