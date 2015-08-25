#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <QImage>
#include <QQuickView>

class Screen : public QObject
{
    Q_OBJECT
public:
    explicit Screen(QQuickView *parent = 0);

signals:

public slots:
     bool save(const QString &path);

private:
    QQuickView *view;

};

#endif // SCREEN_H
