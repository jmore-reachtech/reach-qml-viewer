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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDeclarativeView(parent), m_socket(new QLocalSocket(this)), m_connectTimer(new QTimer(this))
{
    this->setSource(QUrl::fromLocalFile("/home/reach/Projects/reach-apps/demo-qml/content/mainview.qml" ) );
    this->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    this->rootContext()->setContextProperty("connection", this);

    connect(m_socket,SIGNAL(connected()),this,SLOT(onSocketConnected()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(onSocketDisconnected()));
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
    connect(m_socket,SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),this,SLOT(onSocketStateChange(QLocalSocket::LocalSocketState)));
    connect(m_connectTimer, SIGNAL(timeout()), SLOT(onConnectTimerTimeout()));
}

MainWindow::~MainWindow()
{
    qDebug() << "mainwindow destructor";
    if(m_socket->isOpen()) {
        qDebug() << "closing socket";
        m_socket->write("goodbye from Qt");
        m_socket->flush();
        m_socket->close();
    }
    delete m_socket;
}

void MainWindow::init()
{
    tryConnect();
}

void MainWindow::sendMessage(const QString &message)
{
    m_socket->write(message.toLatin1());
}

void MainWindow::onSocketConnected()
{
    qDebug() << "socket connected";
    m_connectTimer->stop();

    m_socket->write("hello from Qt");
}

void MainWindow::onSocketDisconnected()
{
    qDebug() << "socket disconnected";
    m_connectTimer->start(5000);
}

void MainWindow::onSocketError(QLocalSocket::LocalSocketError socketError)
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

void MainWindow::onSocketReadyRead()
{
    qDebug() << "socket ready read";
}

void MainWindow::onSocketStateChange(QLocalSocket::LocalSocketState socketState)
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
            qDebug() << "unknow state"; break;
    }
}

void MainWindow::tryConnect()
{
    m_connectTimer->stop();

    m_socket->connectToServer("/tmp/demo_socket",QIODevice::ReadWrite);
    if (!m_socket->waitForConnected(1000)) {
        m_connectTimer->start(5000);
    }
}

void MainWindow::onConnectTimerTimeout()
{
    tryConnect();
}
