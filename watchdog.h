#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <QObject>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <QDebug>

#define WATCHDOGDEV "/dev/watchdog"

class Watchdog : public QObject
{
    Q_OBJECT
public:
    explicit Watchdog(QObject *parent = 0);
    ~Watchdog();

signals:
    void watchdogError();

public slots:
    bool start();
    void stop();
    bool setInterval(int interval);
    int getInterval();
    bool keepAlive();

private:
    int fd;
    bool m_started;
};

#endif // WATCHDOG_H
