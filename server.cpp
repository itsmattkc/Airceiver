#include "server.h"

#include <QTcpSocket>
#include <QXmlStreamWriter>

const uint64_t Server::kAppleTVFeatures =
      kVideoSupported
    | kPhotoSupported
    | kVideoFairPlay
    | kVideoHTTPLiveStreams
    | kSlideshow
    | kUnknown40
    | kScreenMirrorSupported
    | kScreenRotate
    | kAudioSupported
    | kUnknown400
    | kAudioRedundant
    | kFPSAPv2pt5_AES_GCM
    | kPhotoCaching
    | kAuthentication4
    | kMetadataFeature1
    | kMetadataFeature2
    | kMetadataFeature0
    | kAudioFormat1
    | kAudioFormat2
    | kAudioFormat3
    | kAudioFormat4
    | kUnknown400000
    | kUnknown2000000
    | kSupportsLegacyPairing
    | kRAOP
    | kSupportsAirPlayVideoPlayQueue
    | kSupportsAirPlayFromCloud
    | kUnknown800000000
;

const uint64_t Server::kOurFeatures =
      kScreenMirrorSupported
    | kRAOP
;

const QString Server::g_sourceVersion = QStringLiteral("200.54");
const QString Server::g_deviceModel = QStringLiteral("AppleTV2,1");

Server::Server(QObject *parent)
  : QObject(parent),
    m_zeroconf_error(QZeroConf::noError)
{
  m_server = new QTcpServer(this);
  connect(m_server, &QTcpServer::newConnection, this, &Server::NewConnection);

  m_zeroconf = new QZeroConf(this);
  connect(m_zeroconf, &QZeroConf::error, this, &Server::ZeroConfError);

  // FIXME: Replace with real device ID
  m_device_id = QStringLiteral("30:5A:3A:53:92:92");
}

bool Server::Init()
{
  if (!InitServer()) {
    return false;
  }

  if (!InitBonjour()) {
    return false;
  }

  return true;
}

bool Server::InitServer()
{
  // Initialize TCP server
  if (m_server->listen(QHostAddress::Any, kServerPort)) {
    qDebug() << "Server created at address" << m_server->serverAddress().toString() << "port" << kServerPort;
    return true;
  } else {
    qDebug() << "Failed to create server at address" << m_server->serverAddress().toString() << "port" << kServerPort;
    SetError(tr("Failed to create server at address %1:%2").arg(m_server->serverAddress().toString(), QString::number(kServerPort)));
    return false;
  }
}

bool Server::InitBonjour()
{
  // Publish Bonjour service
  m_zeroconf->addServiceTxtRecord(QStringLiteral("deviceid"), m_device_id);
  m_zeroconf->addServiceTxtRecord(QStringLiteral("features"), GetFeatureBitfield(kOurFeatures));
  m_zeroconf->addServiceTxtRecord(QStringLiteral("model"), g_deviceModel);
  m_zeroconf->addServiceTxtRecord(QStringLiteral("srcvers"), g_sourceVersion);
  m_zeroconf->startServicePublish("Airceiver", "_airplay._tcp", nullptr, kServerPort);
  if (m_zeroconf_error == QZeroConf::noError) {
    qInfo() << "Bonjour service published";
    return true;
  } else {
    const char *bonjour_err = "unknown error code";
    QString bonjour_translated_err = tr("unknown error %1").arg(m_zeroconf_error);

    switch (m_zeroconf_error) {
    case QZeroConf::noError:
    case QZeroConf::browserFailed:
      // Well, this error doesn't make sense, so we'll keep the message as "unknown error code";
      break;
    case QZeroConf::serviceRegistrationFailed:
      bonjour_err = "service registration failed";
      bonjour_translated_err = tr("service registration failed");
      break;
    case QZeroConf::serviceNameCollision:
      bonjour_err = "service name collision";
      bonjour_translated_err = tr("service name collision");
      break;
    }

    qCritical() << "Bonjour service failed to publish:" << bonjour_err << m_zeroconf_error;
    SetError(tr("Bonjour service failed to publish (%1)").arg(bonjour_translated_err));
    return false;
  }
}

void Server::ProcessRequest(QTcpSocket *socket)
{
  const QStringList &lines = m_request_data.take(socket);

  qDebug() << socket->localAddress().toString() << lines.first();

  QStringList intro_segments = lines.first().split(' ');
  const QString &end_point = intro_segments.at(1);

  if (end_point == QStringLiteral("/info")) {
    ResponseInfo(socket);
  } else {
    qCritical() << "Client requested unknown endpoint:" << end_point;
    socket->disconnect();
    emit WarningThrown(tr("Client requested unknown endpoint: %1").arg(end_point));
  }
}

void Server::ResponseInfo(QTcpSocket *socket)
{
  // Create plist with info data
  QByteArray plist;
  QXmlStreamWriter writer(&plist);
  writer.setAutoFormatting(true);
  writer.setCodec("UTF-8");
  writer.writeStartDocument();

  writer.writeDTD(QStringLiteral("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"));

  writer.writeStartElement(QStringLiteral("plist"));
  writer.writeAttribute(QStringLiteral("version"), QStringLiteral("1.0"));

  writer.writeStartElement(QStringLiteral("dict"));

  writer.writeTextElement(QStringLiteral("key"), QStringLiteral("deviceID"));
  writer.writeTextElement(QStringLiteral("string"), m_device_id);

  writer.writeTextElement(QStringLiteral("key"), QStringLiteral("features"));
  writer.writeTextElement(QStringLiteral("integer"), QString::number(kOurFeatures));

  writer.writeTextElement(QStringLiteral("key"), QStringLiteral("sourceVersion"));
  writer.writeTextElement(QStringLiteral("string"), g_sourceVersion);

  writer.writeTextElement(QStringLiteral("key"), QStringLiteral("model"));
  writer.writeTextElement(QStringLiteral("string"), g_deviceModel);

  writer.writeEndElement(); // dict

  writer.writeEndElement(); // plist

  writer.writeEndDocument();

  // Write it out to client
  socket->write(QByteArrayLiteral("RTSP/1.0 200 OK\r\n"));
  socket->write(QStringLiteral("Content-Length: %1\r\n").arg(plist.size()).toUtf8());
  socket->write(QByteArrayLiteral("application/x-apple-binary-plist\r\n"));
  socket->write(QByteArrayLiteral("Server: Airceiver\r\n"));
  socket->write(QByteArrayLiteral("CSeq: 0\r\n"));
  socket->write(QByteArrayLiteral("\r\n"));
  socket->write(plist);

  qDebug() << plist;
}

QString Server::GetFeatureBitfield(uint64_t features)
{
  uint32_t lower = static_cast<uint32_t>(features);
  uint32_t higher = static_cast<uint32_t>(features >> 32);

  QString str = QStringLiteral("0x%1").arg(lower, 0, 16);
  if (higher) {
    str.append(',');
    str.append(QStringLiteral("0x%1").arg(higher, 0, 16));
  }

  return str;
}

void Server::ListFeaturesInBitfield(uint64_t features)
{
  qDebug() << "Bitfield contains the following features:";
  for (uint64_t f=1; f<=kLastFeature; f<<=1) {
    if (features & f) {
      qDebug() << "  " << QString::number(f, 16);
    }
  }
}

void Server::NewConnection()
{
  while (QTcpSocket *skt = m_server->nextPendingConnection()) {
    qInfo() << "Received connection from" << skt->localAddress().toString();

    connect(skt, &QTcpSocket::readyRead, this, static_cast<void(Server::*)()>(&Server::ReadFromSocket));
    connect(skt, &QTcpSocket::disconnected, this, &Server::SocketDisconnected);
  }
}

void Server::ZeroConfError(QZeroConf::error_t error)
{
  m_zeroconf_error = error;
}

void Server::ReadFromSocket()
{
  QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
  while (socket->canReadLine()) {
    QString line = socket->readLine();
    m_request_data[socket].append(line);

    if (line == QStringLiteral("\r\n")) {
      // Request is done
      ProcessRequest(socket);
      break;
    }
  }
}

void Server::SocketDisconnected()
{
  QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
  m_request_data.remove(socket);
  socket->deleteLater();
}
