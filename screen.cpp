#include "screen.h"

Screen::Screen(QDeclarativeView *parent) :
    QObject(0), view(parent)
{
}

bool Screen::save(const QString &path)
{
    return QPixmap::grabWidget(view).save(path, 0, 100);
}
