#include <QApplication>

#include "debug.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  qInstallMessageHandler(DebugHandler);

  QApplication a(argc, argv);

  MainWindow w;
  w.show();

  return a.exec();
}
