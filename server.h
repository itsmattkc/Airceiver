#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QtZeroConf/qzeroconf.h>

class Server : public QObject
{
  Q_OBJECT
public:
  Server(QObject *parent = nullptr);

  // Copied from https://openairplay.github.io/airplay-spec/features.html
  enum AirPlayFeature {
    kVideoSupported = 0x1,
    kPhotoSupported = 0x2,
    kVideoFairPlay = 0x4,
    kVideoVolumeControl = 0x8,
    kVideoHTTPLiveStreams = 0x10,
    kSlideshow = 0x20,
    kUnknown40 = 0x40,
    kScreenMirrorSupported = 0x80,
    kScreenRotate = 0x100,
    kAudioSupported = 0x200,
    kUnknown400 = 0x400,
    kAudioRedundant = 0x800,
    kFPSAPv2pt5_AES_GCM = 0x1000,
    kPhotoCaching = 0x2000,
    kAuthentication4 = 0x4000,
    kMetadataFeature1 = 0x8000,
    kMetadataFeature2 = 0x10000,
    kMetadataFeature0 = 0x20000,
    kAudioFormat1 = 0x40000,
    kAudioFormat2 = 0x80000,
    kAudioFormat3 = 0x100000,
    kAudioFormat4 = 0x200000,
    kUnknown400000 = 0x400000,
    kAuthentication1 = 0x800000,
    kUnknown1000000 = 0x1000000,
    kUnknown2000000 = 0x2000000,
    kHasUnifiedAdvertiserInfo = 0x4000000,
    kSupportsLegacyPairing = 0x8000000,
    kUnknown10000000 = 0x10000000,
    kUnknown20000000 = 0x20000000,
    kRAOP = 0x40000000,
    kUnknown80000000 = 0x80000000,
    kIsCarPlay = 0x100000000,
    kSupportsAirPlayVideoPlayQueue = 0x200000000,
    kSupportsAirPlayFromCloud = 0x400000000,
    kUnknown800000000 = 0x800000000,
    kUnknown1000000000 = 0x1000000000,
    kUnknown2000000000 = 0x2000000000,
    kSupportsCoreUtilsPairingAndEncryption = 0x4000000000,
    kUnknown8000000000 = 0x8000000000,
    kSupportsBufferedAudio = 0x10000000000,
    kSupportsPTP = 0x20000000000,
    kSupportsScreenMultiCodec = 0x40000000000,
    kSupportsSystemPairing = 0x80000000000,
    kUnknown100000000000 = 0x100000000000,
    kUnknown200000000000 = 0x200000000000,
    kSupportsHKPairingAndAccessControl = 0x400000000000,
    kUnknown800000000000 = 0x800000000000,
    kSupportsTransientPairing = 0x1000000000000,
    kUnknown2000000000000 = 0x2000000000000,
    kMetadataFeature4 = 0x4000000000000,
    kSupportsUnifiedPairSetupAndMFi = 0x8000000000000,
    kSupportsSetPeersExtendedMessage = 0x10000000000000,

    kLastFeature = kSupportsSetPeersExtendedMessage
  };

  static const uint64_t kAppleTVFeatures;
  static const uint64_t kOurFeatures;

  bool Init();

  const QString &GetError() const
  {
    return m_error;
  }

  static const int kServerPort = 7000;

signals:
  void WarningThrown(const QString &warning);

private:
  void SetError(const QString &error)
  {
    m_error = error;
  }

  bool InitServer();

  bool InitBonjour();

  void ProcessRequest(QTcpSocket *socket);

  void ResponseInfo(QTcpSocket *socket);

  static QString GetFeatureBitfield(uint64_t features);

  static void ListFeaturesInBitfield(uint64_t features);

  QMap<QTcpSocket *, QStringList> m_request_data;

  QTcpServer *m_server;
  QZeroConf *m_zeroconf;

  QZeroConf::error_t m_zeroconf_error;

  QString m_error;

  QString m_device_id;

  static const QString g_sourceVersion;
  static const QString g_deviceModel;

private slots:
  void NewConnection();

  void ZeroConfError(QZeroConf::error_t error);

  void ReadFromSocket();

  void SocketDisconnected();

};

#endif // SERVER_H
