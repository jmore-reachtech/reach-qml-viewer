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

#include "messagehandler.h"

MessageHandler::MessageHandler(QObject *parent) :
    QObject(parent)
{
}

void MessageHandler::onMessageAvailable(QByteArray ba)
{
    //trim \r\n characters
    ba.replace('\n', "");
    ba.replace('\r', "");

    QByteArray msg(ba);
    //check for empty message
    if (msg.startsWith('\0')) {
        qDebug() << "[QML] message handler: null message";
        return;
    }

    if (msg.contains("=") && msg.contains(".")) {
        QList<QByteArray> value = msg.split('=');
        QList<QByteArray> item = value[0].split('.');

        if(value.count() != 2 && item.count() != 2) {
            emit messageSyntaxError(msg);
        } else {
            emit messageAvailable(QString(item[0]),QString(item[1]),QVariant(value[1]));
            qDebug() << "[QML] item available: " << item[0] << item [1] << value[1];
        }
    } else {
        emit messageSyntaxError(msg);
        qDebug() << "[QML] invalid message: " << msg;
    }
}
