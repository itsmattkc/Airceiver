#include "server.h"

#include <QTcpSocket>

Server::Server(QObject *parent)
  : QObject(parent)
{
  m_server = new QTcpServer(this);
  connect(m_server, &QTcpServer::newConnection, this, &Server::NewConnection);
}

bool Server::Init()
{
  if (!m_server->listen(QHostAddress::Any, kServerPort)) {
    return false;
  }

  return true;
}

void Server::NewConnection()
{
  while (QTcpSocket *skt = m_server->nextPendingConnection()) {
    qDebug() << "Received connection from" << skt->localAddress();
    skt->disconnectFromHost();
  }
}
