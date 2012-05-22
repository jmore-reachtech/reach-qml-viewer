
#include "mainwindow.h"

#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>

#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#include "datamodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0,0,0,0);

  m_qmlScene = new QDeclarativeView;

  m_qmlScene->setResizeMode( QDeclarativeView::SizeRootObjectToView );  

  layout->addWidget(m_qmlScene);

  m_queryConn = new QueryingConnection(this);
  m_queryConn->init("127.0.0.1", 7885);

  m_dataModel = new DataModel(m_queryConn, m_qmlScene, this);
  m_dataModel->setStatusItem("statusText");
  connect (m_dataModel, SIGNAL(ready()), SLOT(onReady()));

  m_upgradeConn = new QueryingConnection(this);
  m_upgradeConn->init("127.0.0.1",7881);

  m_upgradeDataModel = new DataModel(m_upgradeConn, m_qmlScene, this);
  m_upgradeDataModel->setStatusItem("upgradeStatusText");
  //connect (m_upgradeDataModel, SIGNAL(ready()), SLOT(onReady()));
}

void MainWindow::onReady()
{
  m_qmlScene->rootContext()->setContextProperty("connection", qobject_cast<QObject*>(m_queryConn->socket()));
  m_qmlScene->setSource( QUrl::fromLocalFile("content/mainview.qml" ) );

  m_dataModel->setRootObject(m_qmlScene->rootObject());
  m_queryConn->socket()->init();

  m_qmlScene->rootContext()->setContextProperty("upgradeConnection", qobject_cast<QObject*>(m_upgradeConn->socket()));
  m_upgradeDataModel->setRootObject(m_qmlScene->rootObject());
  m_upgradeConn->socket()->init();
}

