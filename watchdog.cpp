#include "watchdog.h"

Watchdog::Watchdog(QObject *parent) :
    QObject(parent)
{
    m_started = false;
}

Watchdog::~Watchdog()
{
    if (m_started)
    {
        stop();
        qDebug() << "watchdog destructor";
    }
}

bool Watchdog::start()
{
    char *dev = WATCHDOGDEV;
    fd = open(dev, O_RDWR);

    if (fd == -1)
    {
        qDebug() << "[QML] Watchdog Error.  Open failed on " << dev;
        return false;
    }

    m_started = true;
    return true;
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
}

bool Watchdog::setInterval(int interval)
{
    if (ioctl(fd, WDIOC_SETTIMEOUT, &interval) != 0) {
        qDebug() << "[QML] Watchdog error : set interval failed.";
        if (m_started)
            stop();
        emit watchdogError();
        return false;
    }

    return true;
}

int Watchdog::getInterval()
{
    int interval;
    if (ioctl(fd, WDIOC_GETTIMEOUT, &interval) == 0) {
        return interval;
    }
    else {
        qDebug() << "[QML] Watchdog error: get interval failed.";
        if (m_started)
            stop();
        emit watchdogError();
    }

    return 0;
}

bool Watchdog::keepAlive()
{
    int size = 0;
    size =  write(fd, "W", 1);
    return size;
}
