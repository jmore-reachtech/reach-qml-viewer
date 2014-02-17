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

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QDebug>
#include <QVariant>

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageHandler(QObject *parent = 0);
    
signals:
    void messageAvailable(const QString &item, const QString &property, const QVariant &value);
    void messageSyntaxError(const QByteArray &msg);

public slots:
    void onMessageAvailable(QByteArray);
    
};

#endif // MESSAGEHANDLER_H
