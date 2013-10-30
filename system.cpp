#include "system.h"

System::System(QObject *parent) : QObject(parent)
  ,m_pwm4_trigger(PWM4_TRIGGER)
  ,m_pwm4_delay_on(PWM4_DELAY_ON)
  ,m_pwm4_delay_off(PWM4_DELAY_OFF)
  ,m_pwm7_trigger(PWM7_TRIGGER)
  ,m_pwm7_delay_on(PWM7_DELAY_ON)
  ,m_pwm7_delay_off(PWM7_DELAY_OFF)
  ,m_volume(0)
  ,m_freq(3000)
  ,m_clock_period(167)
  ,m_pwm_freq(20000)
{
    m_pwm4_trigger.open(QIODevice::ReadWrite);
    m_pwm4_delay_on.open(QIODevice::ReadWrite);
    m_pwm4_delay_off.open(QIODevice::ReadWrite);
    m_pwm7_trigger.open(QIODevice::ReadWrite);
    m_pwm7_delay_on.open(QIODevice::ReadWrite);
    m_pwm7_delay_off.open(QIODevice::ReadWrite);

    //set up periods
    m_period_one_on = (1000000000/(2*m_pwm_freq)/m_clock_period*m_volume/100);
    m_period_one_off = (1000000000/(2*m_pwm_freq)/m_clock_period*(100-m_volume)/100);
    m_period_two = (1000000000/(2*m_freq)/m_clock_period);

    //    QTextStream out(&m_pwm4_trigger);
    //    //set the trigger
    //    if(m_pwm4_trigger.isOpen()) {
    //        qDebug() << "setting m_pwm4_trigger";
    //        out << "timer" << endl;
    //    }

    //    out.setDevice(&m_pwm7_trigger);
    //    if(m_pwm7_trigger.isOpen()) {
    //        out << "timer" << endl;
    //        qDebug() << "setting m_pwm7_trigger";
    //    }

    qDebug() << "m_volume = " << m_volume;
    qDebug() << "m_freq = " << m_volume;
    qDebug() << "m_clock_period = " << m_clock_period;
    qDebug() << "m_pwm_freq = " << m_pwm_freq;
    qDebug() << "m_period_one_on = " << m_period_one_on;
    qDebug() << "m_period_one_off = " << m_period_one_off;
    qDebug() << "m_period_two = " << m_period_two;
}

System::~System()
{
    if(m_pwm4_trigger.isOpen()) {
        m_pwm4_trigger.close();
    }

    if(m_pwm4_delay_on.isOpen()) {
        m_pwm4_delay_on.close();
    }

    if(m_pwm4_delay_off.isOpen()) {
        m_pwm4_delay_off.close();
    }

    if(m_pwm7_trigger.isOpen()) {
        m_pwm7_trigger.close();
    }

    if(m_pwm7_delay_on.isOpen()) {
        m_pwm7_delay_on.close();
    }

    if(m_pwm7_delay_off.isOpen()) {
        m_pwm7_delay_off.close();
    }
}

void System::beep()
{
    /* set volume */
    m_period_one_on = (1000000000/(2*m_pwm_freq)/m_clock_period*m_volume/100);
    m_period_one_off = (1000000000/(2*m_pwm_freq)/m_clock_period*(100-m_volume)/100);
    m_period_two = (1000000000/(2*m_freq)/m_clock_period);

    qDebug() << endl;
    qDebug() << "m_volume = " << m_volume;
    qDebug() << "m_freq = " << m_volume;
    qDebug() << "m_clock_period = " << m_clock_period;
    qDebug() << "m_pwm_freq = " << m_pwm_freq;
    qDebug() << "m_period_one_on = " << m_period_one_on;
    qDebug() << "m_period_one_off = " << m_period_one_off;
    qDebug() << "m_period_two = " << m_period_two;
    qDebug() << endl;

    QTextStream out(&m_pwm4_delay_on);
    out << QString::number(m_period_one_on).toLatin1() << endl;

    out.setDevice(&m_pwm4_delay_off);
    out << QString::number(m_period_one_off).toLatin1() << endl;

    out.setDevice(&m_pwm7_delay_on);
    out << QString::number(m_period_two).toLatin1() << endl;

    out.setDevice(&m_pwm7_delay_off);
    out << QString::number(m_period_two).toLatin1() << endl;

    /* turn down volume */
    m_period_one_on = (1000000000/(2*m_pwm_freq)/m_clock_period*0/100);
    m_period_one_off = (1000000000/(2*m_pwm_freq)/m_clock_period*(100-0)/100);

    systemSleep(100);

    out.setDevice(&m_pwm4_delay_on);
    out << QString::number(m_period_one_on).toLatin1() << endl;

    out.setDevice(&m_pwm4_delay_off);
    out << QString::number(m_period_one_off).toLatin1() << endl;

    out.setDevice(&m_pwm7_delay_on);
    out << QString::number(m_period_two).toLatin1() << endl;

    out.setDevice(&m_pwm7_delay_off);
    out << QString::number(m_period_two).toLatin1() << endl;

    qDebug() << endl;
    qDebug() << "m_volume = " << m_volume;
    qDebug() << "m_freq = " << m_volume;
    qDebug() << "m_clock_period = " << m_clock_period;
    qDebug() << "m_pwm_freq = " << m_pwm_freq;
    qDebug() << "m_period_one_on = " << m_period_one_on;
    qDebug() << "m_period_one_off = " << m_period_one_off;
    qDebug() << "m_period_two = " << m_period_two;
    qDebug() << endl;

}

void System::setVolume(int volume)
{
    m_volume = volume;
}

void System::setFrequency(int freq)
{
    m_freq = freq;
}

void System::systemSleep(int ms)
{
    if(ms <= 0) {return;}

    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}
