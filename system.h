#ifndef SYSTEM_H
#define SYSTEM_H

#include <unistd.h>

#include <QFile>
#include <QObject>
#include <QDebug>
#include <QNetworkInterface>
#include <QTime>
#include <QProcess>

#define BEEPER "/sys/kernel/beeper/beep"
#define VOLUME "/sys/kernel/beeper/vol"
#define FREQUENCY "/sys/kernel/beeper/freq"
#define DURATION "/sys/kernel/beeper/duration"
#define BRIGHTNESS "/sys/devices/platform/mxs-bl.0/backlight/mxs-bl/brightness"
#define BL_POWER "/sys/devices/platform/mxs-bl.0/backlight/mxs-bl/bl_power"
#define DATE_CMD "/bin/date"
#define NETWORK "/etc/network/interfaces"
#define IFUP_CMD "/sbin/ifup"
#define IFDOWN_CMD "/sbin/ifdown"

class System : public QObject
{
    Q_OBJECT
public:
    explicit System(QObject *parent = 0);
    ~System();

public slots:
    void beep();
    void setVolume(int volume);
    int  getVolume(void);
    void setFrequency(int freq);
    int  getFrequency(void);
    void setDuration(int duration);
    int  getDuration(void);
    void setBrightness(int brightness);
    int  getBrightness(void);
    void enableBacklight(void);
    void disableBacklight(void);
    QString getIP(void);
    QDateTime getCurrentDateTime(void);
    bool setDate(int year, int month, int day);
    bool setTime(int hour, int min, int sec);

private:
    QFile m_beeper;
    QFile m_volume;
    QFile m_frequency;
    QFile m_duration;
    QTextStream m_beeper_out;
    QFile m_brightness;
    QFile m_backlight_power;
    QList<QHostAddress> m_ip_list;
    QFile m_network_interfaces;
};

#endif // SYSTEM_H

