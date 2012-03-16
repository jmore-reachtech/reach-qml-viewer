
#include "connectionbase.h"

#include "protocolhandler.h"

#include <QDataStream>
#include <QTcpSocket>
#include <QTimer>

Socket::Socket(QTcpSocket* socket, const QString &hostName, quint16 port, ConnectionBase *conn, QObject* parent)
  : QObject(parent),
    m_socket(socket),
    m_conn(conn),
    m_protocolHandler(new ProtocolHandler(this)),
    m_hostName(hostName),
    m_port(port),
    m_connectTimer(new QTimer(this)),
    m_checkPulseTimer(new QTimer(this)),
    m_verifyPulseTimer(new QTimer(this)),
    m_state(NormalState)
{
  connect(m_socket, SIGNAL(readyRead()), SLOT(read()));
  connect(m_socket, SIGNAL(connected()), SLOT(onConnected()));
  connect(m_socket, SIGNAL(disconnected()), SIGNAL(disconnected()));
  connect(m_socket, SIGNAL(disconnected()), SLOT(tryConnect()));
  connect(m_connectTimer, SIGNAL(timeout()), SLOT(tryConnectInternal()));
  connect(m_checkPulseTimer, SIGNAL(timeout()), SLOT(checkPulse()));
  connect(m_verifyPulseTimer, SIGNAL(timeout()), SLOT(onLostPulse()));

  m_checkPulseTimer->setSingleShot(true);
  m_verifyPulseTimer->setSingleShot(true);
}

static const int connectionTimeouts[] = {
  5,
  10,
  15,
  25,
  40,
  65
};
static int timeoutCounter = 0;

void Socket::onConnected()
{
  m_connectTimer->stop();
  emit connected();
}

void Socket::checkPulse()
{
  m_socket->write("ping\n");
  m_verifyPulseTimer->start(5000);
}

void Socket::onLostPulse()
{
  m_state = ErrorState;
  emit communicationError();
  checkPulse();
}

void Socket::tryConnect()
{
  timeoutCounter = 0;
  QMetaObject::invokeMethod(this, "tryConnectInternal", Qt::QueuedConnection);
}

void Socket::tryConnectInternal()
{
  static const int numTimeouts = sizeof(connectionTimeouts) / sizeof(*connectionTimeouts);
  m_socket->connectToHost(m_hostName, m_port);
  if (!m_socket->waitForConnected(5000)) {
    const int timeout = connectionTimeouts[timeoutCounter++ % numTimeouts];
    emit nextConnectionTimeoutChanged(timeout);
    m_connectTimer->start(1000 * timeout);
  }
  m_checkPulseTimer->start(5000);
}

void Socket::init()
{
  emit ready();
  tryConnect();
}

void Socket::read()
{
  while (m_socket->canReadLine())
  {
    QVector<ProtocolHandler::Change> changes = m_protocolHandler->handleProtocol(m_socket);
    if (!changes.isEmpty())
      m_conn->handleProtocol(changes);
  }
  m_verifyPulseTimer->stop();
  if (m_state == ErrorState) {
    emit connected();
    m_state = NormalState;
  }

  m_checkPulseTimer->start(5000);
}

void Socket::sendMessage(const QString &message)
{
    QMetaObject::invokeMethod(this, "messageHelper", Qt::QueuedConnection,
                              Q_ARG(QString, message));
}

void Socket::messageHelper(const QString &message)
{
    m_socket->write(message.toLatin1() + '\n');
}

void Socket::updateValue(const QString &objectName, const QString &property, const QVariant &value)
{
  QMetaObject::invokeMethod(this, "updateHelper", Qt::QueuedConnection,
                            Q_ARG(QString, objectName),
                            Q_ARG(QString, property),
                            Q_ARG(QVariant, value));
}

void Socket::updateHelper(const QString &objectName, const QString &property, const QVariant &value)
{
  m_socket->write(objectName.toLatin1() + '.' + property.toLatin1() + '=' + value.toByteArray() + '\n');
}

void Socket::query(const QString &objectName, const QString &property)
{

  QMetaObject::invokeMethod(this, "queryHelper", Qt::QueuedConnection,
                            Q_ARG(QString, objectName),
                            Q_ARG(QString, property));
}

void Socket::queryHelper(const QString &objectName, const QString &property)
{
  m_socket->write(objectName.toLatin1() + '.' + property.toLatin1() + "=?\n");
}

ConnectionBase::ConnectionBase(QObject* parent)
  : QThread(parent)
{
}

ConnectionBase::~ConnectionBase()
{
  quit();
  wait();
}

void ConnectionBase::init(const QString& hostName_, qint16 port_)
{
  hostName = hostName_;
  port = port_;
}

void ConnectionBase::login()
{
  socket_->write("LOGIN\n\n");
}

void ConnectionBase::run()
{
  socket_ = new QTcpSocket;
  s = new Socket(socket_, hostName, port, this);

  login();

  exec();
}

