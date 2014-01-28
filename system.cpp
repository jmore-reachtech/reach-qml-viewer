#include "system.h"
#include "systemdefs.h"

System::System(QObject *parent) : QObject(parent)
  ,m_beeper(BEEPER)
  ,m_volume(VOLUME)
  ,m_frequency(FREQUENCY)
  ,m_duration(DURATION)
  ,m_beeper_out()
  ,m_brightness(BRIGHTNESS)
  ,m_backlight_power(BL_POWER)
  ,m_network_interfaces(NETWORK)
{
    m_beeper.open(QIODevice::ReadWrite);
    m_beeper_out.setDevice(&m_beeper);

    m_ip_list = QNetworkInterface::allAddresses();
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

    if(m_brightness.isOpen()) {
        m_brightness.close();
    }

    if(m_backlight_power.isOpen()) {
        m_backlight_power.close();
    }
}

void System::beep()
{
    m_beeper_out << QString::number(1).toLatin1() << endl;
}

void System::setVolume(int volume)
{
    m_volume.open(QIODevice::ReadWrite);
    QTextStream out(&m_volume);

    out << QString::number(volume).toLatin1() << endl;
    m_volume.close();
}

int System::getVolume()
{
    m_volume.open(QIODevice::ReadOnly);
    QTextStream in(&m_volume);
    QString s = in.readLine();
    m_volume.close();

    return s.toInt();
}

void System::setFrequency(int freq)
{
    m_frequency.open(QIODevice::ReadWrite);
    QTextStream out(&m_frequency);

    out << QString::number(freq).toLatin1() << endl;
    m_frequency.close();
}

int System::getFrequency()
{
    m_frequency.open(QIODevice::ReadOnly);
    QTextStream in(&m_frequency);
    QString s = in.readLine();
    m_frequency.close();

    return s.toInt();
}

void System::setDuration(int duration)
{
    m_duration.open(QIODevice::ReadWrite);
    QTextStream out(&m_duration);
    out << QString::number(duration).toLatin1() << endl;
    m_duration.close();
}

int System::getDuration()
{
    m_duration.open(QIODevice::ReadOnly);
    QTextStream in(&m_duration);
    QString s = in.readLine();
    m_duration.close();

    return s.toInt();
}

void System::setBrightness(int brightness)
{
    m_brightness.open(QIODevice::ReadWrite);
    QTextStream out(&m_brightness);
    out << QString::number(brightness).toLatin1() << endl;
    m_brightness.close();
}

int System::getBrightness()
{
    m_brightness.open(QIODevice::ReadOnly);
    QTextStream in(&m_brightness);
    QString s = in.readLine();
    m_brightness.close();

    return s.toInt();
}

void System::enableBacklight()
{
    m_backlight_power.open(QIODevice::ReadWrite);
    QTextStream out(&m_backlight_power);
    out << QString::number(0).toLatin1() << endl;
    m_backlight_power.close();
}

void System::disableBacklight()
{
    m_backlight_power.open(QIODevice::ReadWrite);
    QTextStream out(&m_backlight_power);
    out << QString::number(1).toLatin1() << endl;
    m_backlight_power.close();
}

QString System::getIP()
{
    foreach(QHostAddress ip, m_ip_list) {
        qDebug() << ip.toString();
        if(ip != ip.LocalHost) {
            qDebug() << "returning: " << ip.toString();
            return ip.toString();
        }
    }

    qDebug() << "returning: localhost";
    return "127.0.0.1";
}

QDateTime System::getCurrentDateTime()
{
    qDebug() << "currentDateTime: " << QDateTime::currentDateTime();
    return QDateTime::currentDateTime();
}

bool System::setTime(int hour, int min, int sec)
{
    QDateTime t_cur(QDateTime::currentDateTime());
    QTime t_new(hour,min,sec);
    QString t_set;
    QProcess p(this);
    QString cmd(DATE_CMD);
    QStringList args;

    if (!t_new.isValid()) {
        return false;
    }

    t_set.append(t_cur.date().toString());
    t_set.append(" ");
    t_set.append(t_new.toString());

    args << "-s" << t_set.toLatin1();

    qDebug() << "exec: " << cmd << " " << args;

    p.start(cmd,args);
    p.waitForFinished(1000);

    return true;
}

bool System::setDate(int year, int month, int day)
{
    QDateTime d_cur(QDateTime::currentDateTime());
    QDate d_new(year,month,day);
    QString d_set;
    QProcess p(this);
    QString cmd(DATE_CMD);
    QStringList args;

    if (!d_new.isValid()) {
        return false;
    }

    d_set.append(d_new.toString());
    d_set.append(" ");
    d_set.append(d_cur.time().toString());

    args << "-s" << d_set.toLatin1();

    qDebug() << "exec: " << cmd << " " << args;

    p.start(cmd,args);
    p.waitForFinished(1000);

    return true;
}

