#ifndef SYSTEM_H
#define SYSTEM_H

#include <unistd.h>

#include <QFile>
#include <QObject>
#include <QDebug>

#define PWM4_TRIGGER "/sys/class/leds/beeper-pwm4/trigger"
#define PWM7_TRIGGER "/sys/class/leds/beeper-pwm7/trigger"
#define PWM4_DELAY_ON "/sys/class/leds/beeper-pwm4/delay_on"
#define PWM4_DELAY_OFF "/sys/class/leds/beeper-pwm4/delay_off"
#define PWM7_DELAY_ON "/sys/class/leds/beeper-pwm7/delay_on"
#define PWM7_DELAY_OFF "/sys/class/leds/beeper-pwm7/delay_off"

class System : public QObject
{
    Q_OBJECT
public:
    explicit System(QObject *parent = 0);
    ~System();

public slots:
    void beep();
    void setVolume(int volume);
    void setFrequency(int freq);

private slots:
    void systemSleep(int ms);

private:
    QFile m_pwm4_trigger;
    QFile m_pwm4_delay_on;
    QFile m_pwm4_delay_off;
    QFile m_pwm7_trigger;
    QFile m_pwm7_delay_on;
    QFile m_pwm7_delay_off;
    int m_volume;
    int m_freq;
    int m_clock_period;       // PWM clock is 24Mhz/4 = 167ns
    int m_pwm_freq;           // Volume PWM is fixed at 20Khz, duty cycle varies
    // number of clocks on & off for desired volume
    int m_period_one_on;
    int m_period_one_off;
    // number of clocks in a half cycle for desired frequency
    int m_period_two;
};

#endif // SYSTEM_H
