#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "NDownloader.h"

namespace NulstarNS {
  NDownloader::NDownloader(QObject* rParent) : QObject(rParent) {

  }

  void NDownloader::fDownload(const QUrl& lUrl, const QString& lFilePath) {
  /*  QDir lDir;
    if(lDir.mkpath(lDownloadPath)) {

    }
    else {
      emit sLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, tr("Couldn't create path '%1'. Download aborted.").arg(lDownloadPath));
      return;
    }*/

    QNetworkRequest lDownloadRequest(lUrl);
    QNetworkReply* rNetworkReply = mNetworkManager.get(lDownloadRequest);
    QTimer* rDownloadTimer = new QTimer();
    tDownloadData lDownloadData = {rNetworkReply, QByteArray(), rDownloadTimer, 0};
    
    QString lUrlString(lUrl.toString());
    mDownloadData[lUrlString] = lDownloadData;

    connect(rNetworkReply, &QNetworkReply::downloadProgress, rNetworkReply, [&lUrlString, rNetworkReply, this] (qint64 /*lBytesReceived*/, qint64 /*lBytesTotal*/) {
        mDownloadData[lUrlString].mFileContents.append(rNetworkReply->readAll()); 
        mDownloadData[lUrlString].pTimeoutTimer->stop();
        mDownloadData[lUrlString].pTimeoutTimer->start(cTime_DownloadTimeout * 1000);
      } );
    
    connect(rNetworkReply, &QNetworkReply::finished, rNetworkReply, [&lFilePath, &lUrlString, rNetworkReply, this] () {
        if(rNetworkReply->error() != QNetworkReply::NoError) {
          if(mDownloadData[lUrlString].pTimeoutTimer->isActive())
            mDownloadData[lUrlString].pTimeoutTimer->stop();
          mDownloadData[lUrlString].pTimeoutTimer->deleteLater();
          rNetworkReply->deleteLater();
          mDownloadData.remove(lUrlString);
        }
      } );

    connect(rNetworkReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), rNetworkReply, [&lUrlString, rNetworkReply, this] (QNetworkReply::NetworkError /*lCode*/) {
        emit sLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, tr("File '%1' couldn't be downloaded. Error: '%2'.").arg(lUrlString).arg(rNetworkReply->errorString()));
        emit sError(lUrlString);
      } );

    connect(rDownloadTimer, &QTimer::timeout, rDownloadTimer, [lFilePath, &lUrlString, rDownloadTimer, this] () {
        if(mDownloadData[lUrlString].mRetryTimes == cCount_RetryTimes) {
          rDownloadTimer->stop();
          mDownloadData[lUrlString].pNetworkReply->abort();
          emit sLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, tr("File '%1' couldn't be downloaded succesfully after '%2' retries.").arg(lUrlString).arg(cCount_RetryTimes));
          emit sError(lUrlString);
        }
        else {
          rDownloadTimer->stop();
          mDownloadData[lUrlString].pNetworkReply->abort();
          emit sLog(ELogLevel::eLogWarning, ELogMessageType::eResourceManagement, tr("File '%1' download timeout, retrying ...").arg(lUrlString));
          metaObject()->invokeMethod(this, "fDownload", Qt::QueuedConnection, Q_ARG(QUrl, QUrl(lUrlString)), Q_ARG(QString, lFilePath));
        }
      } );

    rDownloadTimer->start(cTime_DownloadTimeout * 1000);
  }
}
