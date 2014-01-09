#include <QDebug>
#include <QAudioOutput>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

#include "module.h"


Module::Module(QObject *parent) :
    QObject(parent)
{
    m_audio_file.setFileName("/application/src/beep.wav");
    m_audio_file.open(QIODevice::ReadOnly);

    m_media_object = new Phonon::MediaObject(this);
    m_audio_output = new Phonon::AudioOutput(Phonon::NotificationCategory, this);
    m_audio_output->setVolume(0.5);
    m_media_object->setCurrentSource(&m_audio_file);

    Phonon::createPath(m_media_object, m_audio_output);
}

void Module::beep()
{
    qDebug() << "beep";
    m_media_object->setCurrentSource(&m_audio_file);
    m_media_object->play();
}

void Module::setVolume(qreal volume)
{
    m_audio_output->setVolume(volume);
}
