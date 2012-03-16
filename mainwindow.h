
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>

class QDeclarativeView;

class QueryingConnection;
class DataModel;

class MainWindow : public QWidget
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);

private slots:
  void onReady();

private:
  QDeclarativeView *m_qmlScene;
  QueryingConnection *m_queryConn;
  QueryingConnection *m_upgradeConn;
  DataModel *m_dataModel;
  DataModel *m_upgradeDataModel;
};

#endif

