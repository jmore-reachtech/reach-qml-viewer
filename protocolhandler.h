
#ifndef PROTOCOlHANDLER_H
#define PROTOCOlHANDLER_H

#include <QtCore/QObject>
#include <QVector>

#include <QTcpSocket>

class ProtocolHandler : public QObject
{
  Q_OBJECT
public:
  explicit ProtocolHandler(QObject* parent = 0);

  enum Type {
    Invalid,
    LoginOk,
    Dirty,
    ValueResult
  };

  struct Change {
    Change()
      : type(Invalid)
    {

    }
    Change(const Change &other)
      : type(other.type), identifier(other.identifier), property(other.property), value(other.value)
    {

    }

    Change& operator=(const Change &other)
    {
      type = other.type;
      identifier = other.identifier;
      property = other.property;
      value = other.value;
      return *this;
    }

    Type type;
    QString identifier;
    QString property;
    QVariant value;
  };

public:
  /* reimp */ QVector<Change> handleProtocol(QTcpSocket *socket);
};

#endif
