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
    //check for empty message
    if (ba.startsWith('\0')) {
        qDebug() << "[QML] message handler: null message";
        return;
   }

    //trim \r\n characters
    ba.replace('\n', "");
    ba.replace('\r', "");

    QString message(ba);

    if (message.contains("=") && message.contains(".")) {
        QString value;
        int pos = message.indexOf("=");
        value = message.mid(pos + 1, message.length() - (pos + 1));
        QString item = message.mid(0, pos);
        QStringList items = item.split('.');

        if(value.length() == 0 || items.count() != 2) {
            emit messageSyntaxError(ba);
        } else {
            emit messageAvailable(items[0], items[1], QVariant(value));
            qDebug() << "[QML] item available: " << items[0] << items[1] << value;
        }
    } else {
        emit messageSyntaxError(ba);
        qDebug() << "[QML] invalid message: " << message;
    }
}
