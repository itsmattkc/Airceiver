#include "mainwindow.h"

#include "server.h"
#include "qtutils.h"

#define super QMainWindow

MainWindow::MainWindow(QWidget *parent)
  : super(parent),
    m_first_show(true)
{
  setWindowTitle(tr("Airceiver"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::showEvent(QShowEvent *event)
{
  super::showEvent(event);

  if (m_first_show) {
    Server *server = new Server(this);
    if (!server->Init()) {
      QtUtils::MessageBox(this, tr("Failed to initialize server"), tr("Failed to initialize server on port %1.").arg(Server::kServerPort));
    }

    m_first_show = false;
  }
}

