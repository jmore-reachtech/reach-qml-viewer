#include "messagehandler.h"

MessageHandler::MessageHandler(QObject *parent) :
    QObject(parent)
{
}

void MessageHandler::onMessageAvailable(QByteArray ba)
{
    QByteArray msg(ba.trimmed());

    //check for empty message
    if (msg.startsWith('\0')) {
        qDebug() << "message handler: null message";
        return;
    }

    // this for our heartbeat
    if(msg == "pong") {
        qDebug() << "got hearbeat";
        return;
    }

    if (msg.contains("=") && msg.contains(".")) {
        QList<QByteArray> value = msg.split('=');
        QList<QByteArray> item = value[0].split('.');

        emit itemAvailable(QString(item[0]),QString(item[1]),QVariant(value[1]));
        qDebug() << "item lookup: " << item[0] << item [1] << value[1];
    } else {
        qDebug() << "invalid message: " << msg;
    }
}
