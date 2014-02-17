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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QtNetwork>
#include <QTimer>

class MainWindow : public QDeclarativeView
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();

public slots:
    void sendMessage(const QString &message);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QLocalSocket::LocalSocketError);
    void onSocketReadyRead();
    void onSocketStateChange(QLocalSocket::LocalSocketState );
    void tryConnect();
    void onConnectTimerTimeout();
    
private:
    QLocalSocket *m_socket;
    QTimer *m_connectTimer;
};

#endif // MAINWINDOW_H
