#include "screen.h"

Screen::Screen(QQuickView *parent) :
    QObject(0), view(parent)
{
}

bool Screen::save(const QString &path)
{
    QImage image = view->grabWindow();
    return image.save(path, 0, 80);
}
