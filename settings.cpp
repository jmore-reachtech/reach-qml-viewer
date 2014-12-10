#include "settings.h"
#include <QSettings>
#include <QDebug>

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

Settings::~Settings()
{
    qDebug() << "[QML] settings destructor";
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    QSettings settings(APPLICATION_SETTINGS_FILE,QSettings::NativeFormat);
    settings.beginGroup(APPLICATION_SETTINGS_SECTION);
    settings.setValue(key, value);
    qDebug() << "[QML] set setting key: " << key << ":" << value ;
    settings.endGroup();
}

QVariant Settings::getValue(const QString &key, const QVariant &defaultValue) const
{
    QVariant val;
    QSettings settings(APPLICATION_SETTINGS_FILE,QSettings::NativeFormat);
    settings.beginGroup(APPLICATION_SETTINGS_SECTION);
    val = settings.value(key, defaultValue);
    qDebug() << "[QML] get setting key: " << key << ":" << val;
    settings.endGroup();
    return val;
}

void Settings::remove(const QString &key)
{
    QSettings settings(APPLICATION_SETTINGS_FILE,QSettings::NativeFormat);
    settings.beginGroup(APPLICATION_SETTINGS_SECTION);
    settings.remove(key);
    qDebug() << "[QML] remove setting key: " << key;
    settings.endGroup();
}
