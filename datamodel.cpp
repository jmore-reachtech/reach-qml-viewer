
#include "datamodel.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeEngine>

DataModel::DataModel(QueryingConnection *queryConn_, QDeclarativeView *view, QObject* parent)
  : QObject(parent),
    m_view(view),
    queryConn(queryConn_),
    m_statusText("statusText")
{
  connect(queryConn_, SIGNAL(identifierValued(QString,QString,QVariant)), this, SLOT(identifierValued(QString,QString,QVariant)));

  connect(queryConn_, SIGNAL(started()), SLOT(started()));
  queryConn_->start();
}

void DataModel::setStatusItem(const QString statusItem)
{
    m_statusText = statusItem;
}

QString DataModel::getStatusItem() const
{
    return m_statusText;
}

void DataModel::setRootObject(QGraphicsObject *rootItem_)
{
  rootItem = rootItem_;
}

void DataModel::started()
{
  disconnect(queryConn, SIGNAL(started()), this, SLOT(started()));
  connect(queryConn->socket(), SIGNAL(connected()), SLOT(onConnected()));
  connect(queryConn->socket(), SIGNAL(disconnected()), SLOT(onDisconnected()));
  connect(queryConn->socket(), SIGNAL(communicationError()), SLOT(onCommunicationError()));
  emit ready();
}

void DataModel::onConnected()
{
  identifierValued(m_statusText, "text", "Connected");
  emit connected();
}

void DataModel::onDisconnected()
{
  identifierValued(m_statusText, "text", "Not Connected");
  emit disconnected();
}

void DataModel::onCommunicationError()
{
  identifierValued(m_statusText, "text", "Communication Error");
  emit communicationError();
}

void DataModel::identifierValued(const QString &identifier, const QString &property, const QVariant &value)
{
  QDeclarativeItem *obj = rootItem->findChild<QDeclarativeItem*>(identifier.toLatin1());
  if (!obj) {
    qWarning() << "No object with name :" << identifier;
    return;
  }
  if (value.type() == QVariant::String)
  {
    {
      QFileInfo fi = m_view->source().toString();

      QFile f;
      f.setFileName(fi.absolutePath() + QDir::separator() + value.toString());
      if (f.exists()) {
        obj->setProperty(property.toLatin1(), QUrl::fromLocalFile(f.fileName()));
        return;
      }
    }
    foreach (const QString &importPath, m_view->engine()->importPathList()) {
      QFile f;
      f.setFileName(importPath + value.toString());
      if (f.exists()) {
        obj->setProperty(property.toLatin1(), QUrl::fromLocalFile(f.fileName()));
        return;
      }
    }
  }
  obj->setProperty(property.toLatin1(), value);
}

