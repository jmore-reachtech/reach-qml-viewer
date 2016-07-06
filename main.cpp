/*************************************************************************
**
**    (C) Copyright 2013 Reach Technology Inc.
**
**    This code is protected by international copyright laws. This file may
**    only be used in accordance with a license and cannot be used on
**    hardware other than supplied by Reach Technology Inc. We appreciate
**    your understanding and fairness.
**
*************************************************************************/
#include <QGuiApplication>
#include <QSettings>
#include <QFileInfo>
#include <signal.h>
#include "mainview.h"
#include "systemdefs.h"

static void unixSignalHandler(int signum) {
    qDebug("[QML] main.cpp::unixSignalHandler(). signal = %s\n", strsignal(signum));

    /*
     * Make sure your Qt application gracefully quits.
     * NOTE - purpose for calling qApp->exit(0):
     *      1. Forces the Qt framework's "main event loop `qApp->exec()`" to quit looping.
     *      2. Also emits the QGuiApplication::aboutToQuit() signal. This signal is used for cleanup code.
     */
    qApp->exit(0);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Reach Technology");
    QGuiApplication::setOrganizationDomain("reachtech.com");
    QGuiApplication::setApplicationName("Qml Viewer");
    QGuiApplication::setApplicationVersion(APP_VERSION);
    QFileInfo settingsFile;

    QStringList args = app.arguments();

    foreach (QString item, args) {
        if(item == "--version" || item == "-v") {
            qDebug() << "QML Viewer " << APP_VERSION;
            return 0;
        }
    }

    QString sb(QGuiApplication::applicationDirPath());
    sb.append(QDir::separator());
    sb.append("settings.conf");
    // check to see if we have a settings file where we started from
    // if not fall back to system hard coded path
    QFileInfo file(sb.toLatin1());
    if (file.exists()) {
        qDebug() << "[QML] using local settings file";
        settingsFile.setFile(file.filePath());
    } else {
        qDebug() << "[QML] using system defined settings file";
        settingsFile.setFile(SYSTEM_SETTINGS_FILE);
    }

    QSettings settings(settingsFile.filePath(),QSettings::NativeFormat);

    settings.beginGroup(SYSTEM_SETTINGS_SECTION);

    MainView w;
    w.setSource(QUrl::fromLocalFile(settings.value("main_view").toString()));
    w.enableLookupAck(settings.value("enable_ack",false).toBool());
    w.setResizeMode(QQuickView::SizeRootObjectToView);

    if (settings.value("full_screen",false).toBool()) {
        w.showFullScreen();
    }

    if (settings.value("hide_curosr",false).toBool()) {
        w.setCursor(QCursor( Qt::BlankCursor ));
    }

    settings.endGroup();

    /* Set a signal handler for a power down or a control-c */
    if (signal(SIGTERM, unixSignalHandler) == SIG_ERR) {
        qDebug() << "[QML] an error occurred while setting a signal terminate handler";
    }
    if (signal(SIGINT, unixSignalHandler) == SIG_ERR) {
        qDebug() << "[QML] an error occurred while setting a signal interrupt handler";
    }
    QObject::connect(&app, SIGNAL(aboutToQuit()), &w, SLOT(handleSigTerm()) );

    w.show();
    return app.exec();
}
