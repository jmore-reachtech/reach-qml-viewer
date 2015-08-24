#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <QPixmap>
#include <QDeclarativeView>

class Screen : public QObject
{
    Q_OBJECT
public:
    explicit Screen(QDeclarativeView *parent = 0);

signals:

public slots:
     bool save(const QString &path);

private:
    QDeclarativeView *view;

};

#endif // SCREEN_H
