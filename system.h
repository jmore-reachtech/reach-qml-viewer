#ifndef SYSTEM_H
#define SYSTEM_H

#include <unistd.h>

#include <QFile>
#include <QObject>
#include <QDebug>

#define BEEPER "/sys/kernel/beeper/beep"
#define VOLUME "/sys/kernel/beeper/vol"
#define FREQUENCY "/sys/kernel/beeper/freq"
#define DURATION "/sys/kernel/beeper/duration"

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

private:
    QFile m_beeper;
    QFile m_volume;
    QFile m_frequency;
    QFile m_duration;
    QTextStream m_beeper_out;
};

#endif // SYSTEM_H
