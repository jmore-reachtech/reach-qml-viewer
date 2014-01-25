#include "system.h"

System::System(QObject *parent) : QObject(parent)
  ,m_beeper(BEEPER)
  ,m_volume(VOLUME)
  ,m_frequency(FREQUENCY)
  ,m_duration(DURATION)
  ,m_beeper_out()
{
    m_beeper.open(QIODevice::ReadWrite);
    m_volume.open(QIODevice::ReadWrite);
    m_frequency.open(QIODevice::ReadWrite);
    m_duration.open(QIODevice::ReadWrite);

    m_beeper_out.setDevice(&m_beeper);
}

System::~System()
{
    if(m_beeper.isOpen()) {
        m_beeper.close();
    }

    if(m_volume.isOpen()) {
        m_volume.close();
    }

    if(m_frequency.isOpen()) {
        m_frequency.close();
    }

    if(m_duration.isOpen()) {
        m_duration.close();
    }
}

void System::beep()
{
    m_beeper_out << QString::number(1).toLatin1() << endl;
}

void System::setVolume(int volume)
{
    QTextStream out(&m_volume);
    out << QString::number(volume).toLatin1() << endl;
}

int System::getVolume()
{
    QTextStream in(&m_volume);
    QString s = in.readLine();

    return s.toInt();
}

void System::setFrequency(int freq)
{
    QTextStream out(&m_frequency);
    out << QString::number(freq).toLatin1() << endl;
}

int System::getFrequency()
{
    QTextStream in(&m_frequency);
    QString s = in.readLine();

    return s.toInt();
}

void System::setDuration(int duration)
{
    QTextStream out(&m_duration);
    out << QString::number(duration).toLatin1() << endl;
}

int System::getDuration()
{
    QTextStream in(&m_duration);
    QString s = in.readLine();

    return s.toInt();
}

