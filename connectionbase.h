
#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H

#include "protocolhandler.h"

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

#include <QTcpSocket>

class Socket;
class QTimer;

class ConnectionBase : public QThread
{
  Q_OBJECT
public:
  explicit ConnectionBase(QObject* parent = 0);
  virtual ~ConnectionBase();

  void init(const QString &hostName, qint16 port);

  Socket* socket() const { return s; }
public slots:

  virtual void handleProtocol(QVector<ProtocolHandler::Change> changes) = 0;


protected:
  /* reimp */ void run();

signals:
  void error(int socketError, const QString &message);

private:
  void login();

private:
  QString hostName;
  quint16 port;

  QTcpSocket *socket_;
  Socket *s;
};

class Socket : public QObject
{
  Q_OBJECT
public:
  enum State {
    NormalState,
    ErrorState
  };
  explicit Socket(QTcpSocket *socket, const QString &hostName, quint16 port, ConnectionBase *conn, QObject* parent = 0);

  void init();
signals:
  void ready();
  void connected();
  void disconnected();
  void communicationError();
  void nextConnectionTimeoutChanged(int timeout);

public slots:
  void query(const QString &objectName, const QString &property);

  void updateValue(const QString &objectName, const QString &property, const QVariant &value);
  void sendMessage(const QString &message);
  void tryConnect();
  void onConnected();

private slots:
  void checkPulse();
  void onLostPulse();
  void tryConnectInternal();
  void read();
  void queryHelper(const QString &objectName, const QString &property);
  void updateHelper(const QString &objectName, const QString &property, const QVariant &value);
  void messageHelper(const QString &message);

private:
  QTcpSocket *m_socket;
  ConnectionBase *m_conn;
  ProtocolHandler *m_protocolHandler;
  QString m_hostName;
  quint16 m_port;
  QTimer *m_connectTimer;
  QTimer *m_checkPulseTimer;
  QTimer *m_verifyPulseTimer;
  State m_state;
};

#endif
