
#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QtCore/QObject>
#include <QGraphicsObject>
#include <QtDeclarative/QDeclarativeView>

#include "queryingconnection.h"

class DataModel : public QObject
{
  Q_OBJECT
public:
  explicit DataModel(QueryingConnection *queryConn, QDeclarativeView *view, QObject* parent = 0);

  void setRootObject(QGraphicsObject *rootItem);
  void setStatusItem(const QString statusItem);
  QString getStatusItem() const;

signals:
  void ready();
  void connected();
  void disconnected();
  void communicationError();

public slots:
  void identifierValued(const QString &meterName, const QString &property, const QVariant &value);

private slots:
  void started();
  void onConnected();
  void onDisconnected();
  void onCommunicationError();

private:
  QueryingConnection *queryConn;
  QGraphicsObject *rootItem;
  QDeclarativeView *m_view;
  QString m_statusText;
};

#endif
