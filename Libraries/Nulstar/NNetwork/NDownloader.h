#ifndef NDOWNLOADER_H
#define NDOWNLOADER_H

#include <QMap>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QUrl>

#include "NNetwork.h"

class QNetworkReply;

namespace NulstarNS {
  const quint8 cTime_DownloadTimeout = 5;
  const quint8 cCount_RetryTimes = 3; //Retry automatically without emitting an error signal

  class NETWORKSHARED_EXPORT NDownloader : public QObject {
    Q_OBJECT

    public:
      explicit NDownloader(QObject* rParent = nullptr);

      void fDownload(QUrl, quint8 lRetryTimes = cCount_RetryTimes);

    private:
      QNetworkAccessManager mNetworkManager;
      QMap<QString, QNetworkReply*> mReplyList; // Url, NetworkReply
  };
}

#endif // NDOWNLOADER_H
