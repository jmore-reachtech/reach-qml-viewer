#ifndef Module_H
#define Module_H

#include <QObject>
#include <QAudioOutput>
#include <QFile>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

class Module : public QObject
{
    Q_OBJECT
public:
    explicit Module(QObject *parent = 0);
    
signals:
    
public slots:
    void beep(void);
    void setVolume(qreal volume);

private slots:

private:
    QFile m_audio_file;
    Phonon::MediaObject *m_media_object;
    Phonon::AudioOutput *m_audio_output;
    
};

#endif // Module_H
