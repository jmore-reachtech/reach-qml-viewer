
#include "protocolhandler.h"
#include <QTcpSocket>
#include <QStringList>

ProtocolHandler::ProtocolHandler(QObject* parent)
  : QObject(parent)
{

}

QVector<ProtocolHandler::Change> ProtocolHandler::handleProtocol(QTcpSocket* socket)
{
  QVector<ProtocolHandler::Change> changes;

  while (socket->canReadLine())
  {
    QByteArray ba = socket->readLine().trimmed();
    qDebug() << ba;
    if (ba == "pong!") {
      continue;
    }
    if (ba == "OK") {
      Change change;
      change.type = LoginOk;
      changes.append(change);
    } else if (!ba.contains("=")) {
      /* sanity check */
      if (!ba.contains('.'))
        continue;

      Change change;
      change.type = Dirty;
      QList<QByteArray> parts = ba.split('.');
      change.identifier = parts[0];
      change.property = parts[1];
      changes.append(change);
    } else {
      Change change;
      change.type = ValueResult;

      /* sanity check */
      if (!ba.contains('.'))
        continue;

      QList<QByteArray> parts = ba.split('=');
      QList<QByteArray> objectParts = parts[0].split('.');
      change.identifier = objectParts[0];
      change.property = objectParts[1];
      QVariant v = parts[1];

      QStringList onList;
      onList << "on" << "true";
      QStringList offList;
      offList << "off" << "false";

      if (onList.contains(QString(parts[1]))) {
        change.value = true;
      } else if (offList.contains(QString(parts[1]))) {
        change.value = false;
      } else if (v.canConvert<int>() && v.convert(QVariant::Int)) {
        change.value = v;
      } else {
        change.value = QString::fromLatin1(parts[1]);
      }
      changes.append(change);
    }
  }
  return changes;
}
