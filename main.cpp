
#include <QtGui/QApplication>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  bool m_maximized = true;
  bool m_enableCursor = false;

  QStringList args = app.arguments();

  foreach (QString item, args) {
      if(item == "--disable-full-screen") {m_maximized = false;}
      if(item == "--enable-cursor") {m_enableCursor = true;}
  }

  MainWindow mw;
  if(m_maximized) {
      mw.showFullScreen();
  }
  if(! m_enableCursor) {
      mw.setCursor(QCursor( Qt::BlankCursor ));
  }
  mw.show();

  return app.exec();
}

