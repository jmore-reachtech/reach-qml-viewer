#include "watchdog.h"

Watchdog::Watchdog(QObject *parent) :
    QObject(parent)
{
    m_started = false;
}

Watchdog::~Watchdog()
{
}

bool Watchdog::start()
{
    const char *dev = QByteArray(WATCHDOGDEV).constData();
    fd = open(dev, O_RDWR);

    if (fd == -1)
    {
        qDebug() << "[QML] Watchdog Error.  Open failed on " << dev;
        emit watchdogError(QString("Watchdog open failed on ").append(dev));
        return false;
    }

    qDebug() << "[QML] starting watchdog timer";
    m_started = true;
    return true;
}

bool Watchdog::isStarted()
{
    return m_started;
}

void Watchdog::stop()
{
    /* The 'V' value needs to be written into watchdog device file to indicate
          that we intend to close/stop the watchdog. Otherwise, debug message
          'Watchdog timer closed unexpectedly' will be printed
    */
    write(fd, "V", 1);
    /* Closing the watchdog device will deactivate the watchdog. */
    close(fd);
    m_started = false;
    qDebug() << "[QML] stopped watchdog timer";
}

bool Watchdog::setInterval(int interval)
{
    if (interval < 30)
    {
        qDebug() << "[QML] Watchdog error : set interval failed.  Interval must be greater than 30 seconds.";
        emit watchdogError("Interval must be greater than 30 seconds");
        return false;
    }

    if (ioctl(fd, WDIOC_SETTIMEOUT, &interval) != 0) {
        qDebug() << "[QML] Watchdog error : set interval failed.";
        if (m_started)
            stop();
        emit watchdogError("Set interval failed.  Watchdog may not have been started.");
        return false;
    }

    qDebug() << "[QML] watchdog set interval: " << interval;
    return true;
}

int Watchdog::getInterval()
{
    int interval;
    if (ioctl(fd, WDIOC_GETTIMEOUT, &interval) == 0) {
        return interval;
    }
    else {
        qDebug() << "[QML] watchdog error: get interval failed";
        if (m_started)
            stop();
        emit watchdogError("Get interval failed.  Watchdog may not have been started.");
    }

    return 0;
}

bool Watchdog::keepAlive()
{
    int size = 0;
    size =  write(fd, "W", 1);
    qDebug() << "[QML] watchdog kicked.";
    return size;
}

bool Watchdog::lastBootByWatchDog()
{
    int bootstatus;
    if (ioctl(fd, WDIOC_GETBOOTSTATUS, &bootstatus) == 0) {
        if (bootstatus != 0)
            return true;
    }
    else{
        qDebug() << "[QML] watchdog error: get boot status failed.";
        if (m_started)
            stop();
        emit watchdogError("Get boot status failed.  Watchdog may not have been started.");
    }

    return false;
}
