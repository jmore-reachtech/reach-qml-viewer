#include <QApplication>
#include <QSettings>

#include "mainview.h"
#include "systemdefs.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setOrganizationName("Reach Technology");
    QApplication::setOrganizationDomain("reachtech.com");
    QApplication::setApplicationName("Qml Viewer");
    QSettings::setPath(QSettings::NativeFormat,QSettings::SystemScope,".");

    QSettings settings(SYSTEM_SETTINGS_FILE,QSettings::NativeFormat);
    settings.beginGroup(SYSTEM_SETTINGS_SECTION);

    MainView w;
    w.setSource(QUrl::fromLocalFile(settings.value("main_view").toString()));
    w.enableLookupAck(settings.value("enable_ack",false).toBool());
    w.enableHeartbeat(settings.value("enable_heartbeat",false).toBool());
    w.setResizeMode(QDeclarativeView::SizeRootObjectToView);

    if (settings.value("full_screen",true).toBool()) {
        w.showFullScreen();
    }

    if (settings.value("hide_curosr",true).toBool()) {
        w.setCursor(QCursor( Qt::BlankCursor ));
    }

    settings.endGroup();

    w.show();
    
    return app.exec();
}
