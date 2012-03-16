
#include "queryingconnection.h"

#include <QTcpSocket>

QueryingConnection::QueryingConnection(QObject* parent)
  : ConnectionBase(parent), m_protocolHandler(new ProtocolHandler(this))
{

}

void QueryingConnection::handleProtocol(QVector<ProtocolHandler::Change> changes)
{
  foreach(const ProtocolHandler::Change change, changes)
  {
    /* sanity check */
    if (change.type != ProtocolHandler::ValueResult)
      continue;

    identifierValued(change.identifier, change.property, change.value);
  }
}

