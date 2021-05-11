#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QObject
{
  Q_OBJECT
public:
  Server(QObject *parent = nullptr);

  bool Init();

  static const int kServerPort = 7000;

private:
  QTcpServer *m_server;

private slots:
  void NewConnection();

};

#endif // SERVER_H
