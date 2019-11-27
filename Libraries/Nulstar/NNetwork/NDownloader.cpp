#include <QNetworkRequest>
#include <QNetworkReply>

#include "NDownloader.h"

namespace NulstarNS {
  NDownloader::NDownloader(QObject* rParent) : QObject(rParent) {

  }

  void NDownloader::fDownload(QUrl lUrl) {
    QNetworkRequest lDownloadRequest(lUrl);
    QNetworkReply* rNetworkReply = mNetworkManager.get(lDownloadRequest);

   /*   connect(_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(processDownloadProgress(qint64,qint64)));
      connect(_reply, SIGNAL(finished()), this, SLOT(processDownloadFinished()));
      connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onDownloadError(QNetworkReply::NetworkError)));

      connect(reply, &QIODevice::readyRead, this, &MyClass::slotReadyRead);
      connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
              this, &MyClass::slotError);
      connect(reply, &QNetworkReply::sslErrors,
              this, &MyClass::slotSslErrors);*/
  }
}
