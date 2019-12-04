#ifndef NDOWNLOADER_H
#define NDOWNLOADER_H

#include <QByteArray>
#include <QMap>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QTimer>

#include <NLogger.h>
#include "NNetwork.h"

class QNetworkReply;

namespace NulstarNS {
  const quint8 cTime_DownloadTimeout = 5;
  const quint8 cCount_RetryTimes = 3; //Retry automatically without emitting an error signal

  struct tDownloadData {
    QNetworkReply* pNetworkReply;
    QByteArray mFileContents;
    QTimer* pTimeoutTimer;
    quint8 mRetryTimes;
  };

  class NETWORKSHARED_EXPORT NDownloader : public QObject {
    Q_OBJECT

    public:
      explicit NDownloader(QObject* rParent = nullptr);

      Q_INVOKABLE void fDownload(const QUrl &lUrl, const QString &lFilePath);

    private:
      QNetworkAccessManager mNetworkManager;
      QMap<QString, tDownloadData> mDownloadData; // Url, DownloadData

    signals:
      void sLog(ELogLevel eLogLevel, ELogMessageType eLogMessageType, const QString& lMessage);
      void sError(const QUrl& lUrl);
  };
}

#endif // NDOWNLOADER_H
