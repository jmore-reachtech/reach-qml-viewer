#include "screen.h"
#include <QDebug>

Screen::Screen(QQuickView *parent) :
    QObject(0), view(parent), m_screenSaverTimer(new QTimer(this))
{
    m_screenSaverEnabled = false;
    QSettings settings(SYSTEM_SETTINGS_FILE,QSettings::NativeFormat);
    settings.beginGroup(SYSTEM_SETTINGS_SECTION);
    m_screenSaverTimeout = settings.value("screensaver_timeout",0).toInt();\
    m_screenOriginalBrightness = settings.value("screen_original_brigtness",7).toInt();
    m_screenDimBrightness = settings.value("screen_dim_brigtness",5).toInt();
    settings.endGroup();
    m_dim = false;

    if (m_screenSaverTimeout > 0)
    {
        m_screenSaverEnabled = true;
        //set the original brighness in case the device was shutdown in dim mode
        setBrightness(m_screenOriginalBrightness);
        connect(m_screenSaverTimer, SIGNAL(timeout()),this,SLOT(onScreenSaverTimerTimeout()));
        view->installEventFilter(this);
        m_screenSaverTimer->start(m_screenSaverTimeout * 60 * 1000);
    }
}

bool Screen::save(const QString &path)
{
    //check if the path folder exists
    QStringList parts = path.split("/");
    QString folder;

    if (parts.length() > 0)
        folder = parts.at(0);
    for (int i=1; i < parts.length()-1; i++)
        folder.append("/" + parts.at(i));

    QDir dir(folder);

    if (!dir.exists())
    {
        if (dir.mkpath(folder))
            qDebug() << "[QML] created folder" << folder << "for screen save.";
        else
        {
            qDebug() << "{QML] unable to create folder for screen save." << folder << "make sure path is correct." << path;
            return false;
        }
    }

    QImage image = view->grabWindow();
    if (image.save(path, 0, 80))
    {
        qDebug() << "[QML] screen save successful at" << path;
        return true;
    }
    else
    {
        qDebug() << "[QML] screen save failed at" << path;
        return false;
    }
}

void Screen::setOriginalBrightness()
{
    setBrightness(m_screenOriginalBrightness);
    m_dim = false;
    m_screenSaverTimer->stop();
    m_screenSaverTimer->start(m_screenSaverTimeout * 60 * 1000);
}

bool Screen::isDim()
{
    return m_dim;
}

bool Screen::isScreenSaverEnabled()
{
    return m_screenSaverEnabled;
}

void Screen::onScreenSaverTimerTimeout()
{
    setBrightness(m_screenDimBrightness);
    m_dim = true;
}

void Screen::setBrightness(int val)
{
    QFile brightness_file(BRIGHTNESS);
    brightness_file.open(QIODevice::ReadWrite);
    QTextStream out(&brightness_file);

    out << QString::number(val).toLatin1() << endl;
    brightness_file.close();

}

bool Screen::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    switch (event->type()) {
    case QEvent::KeyRelease:
    case QEvent::KeyPress:
    case QEvent::MouseButtonPress:
    case QEvent::TouchBegin:
    case QEvent::MouseMove:
        if (m_dim)
            return true;
    case QEvent::MouseButtonRelease:
        if (m_dim)
        {
            setOriginalBrightness();
            return true;
        }
        else
        {
            m_screenSaverTimer->stop();
            m_screenSaverTimer->start(m_screenSaverTimeout * 60 * 1000);
        }
        break;
    default:
        break;
    }

    return false;
}
