
#ifndef QUERYINGCONNECTION_H
#define QUERYINGCONNECTION_H

#include "connectionbase.h"

#include "protocolhandler.h"

class QueryingConnection : public ConnectionBase
{
  Q_OBJECT
public:
  explicit QueryingConnection(QObject* parent = 0);

signals:
  void identifierValued(const QString &identifier, const QString &property, const QVariant &value);

public slots:
  /* reimp */ void handleProtocol(QVector<ProtocolHandler::Change> changes);

private:
  ProtocolHandler *m_protocolHandler;
};

#endif
