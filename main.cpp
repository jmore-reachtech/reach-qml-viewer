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

#include "mainview.h"
#include "systemdefs.h"

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

    w.show();
    
    return app.exec();
}
