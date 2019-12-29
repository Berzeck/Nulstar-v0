#include <QDir>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "NDownloader.h"

namespace NulstarNS {
  NDownloader::NDownloader(QObject* rParent) : QObject(rParent) {
    mNetworkManager = new QNetworkAccessManager();
  }
  NDownloader::~NDownloader() {
    mNetworkManager->deleteLater();
  }

  void NDownloader::fDownload(const QUrl& lUrl, const QString& lFilePath, bool lNotifyIfSuccessfull, const QString& lID ) {
    QString lUrlString(lUrl.toString());
    QString lDownloadID(lID);
    if(lID.simplified().isEmpty())
      lDownloadID = lUrlString;

    if(mDownloadData.contains(lDownloadID))
      return;
    if(!lFilePath.isEmpty()) {
      QFile lTargetFile(lFilePath);
      if(lTargetFile.exists() && !lTargetFile.remove()) {
        emit sLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, tr("File '%1' exists but couldn't be removed.").arg(lUrlString));
        emit sError(lDownloadID);
        return;
      }
    }
    QNetworkRequest lDownloadRequest(lUrl);
    QNetworkReply* rNetworkReply = mNetworkManager->get(lDownloadRequest);
    QTimer* rDownloadTimer = new QTimer();
    tDownloadData lDownloadData = {rNetworkReply, QByteArray(), rDownloadTimer, 0, lNotifyIfSuccessfull};

    mDownloadData[lDownloadID] = lDownloadData;

    connect(rNetworkReply, &QNetworkReply::downloadProgress, rNetworkReply, [lDownloadID, rNetworkReply, this] (qint64 /*lBytesReceived*/, qint64 /*lBytesTotal*/) {
        mDownloadData[lDownloadID].mFileContents.append(rNetworkReply->readAll());
        mDownloadData[lDownloadID].pTimeoutTimer->stop();
        mDownloadData[lDownloadID].pTimeoutTimer->start(cTime_DownloadTimeout * 1000);
      } );
    
    connect(rNetworkReply, &QNetworkReply::finished, rNetworkReply, [lFilePath, lDownloadID, lUrlString, rNetworkReply, this] () {
        if(mDownloadData[lDownloadID].pTimeoutTimer->isActive())
          mDownloadData[lDownloadID].pTimeoutTimer->stop();

        if(rNetworkReply->error() == QNetworkReply::NoError) {
          if(lFilePath.isEmpty()) {
            if(mDownloadData[lDownloadID].mNotifyIfSuccessfull)
              emit sLog(ELogLevel::eLogInfo, ELogMessageType::eResourceManagement, tr("File '%1' downloaded successfully.").arg(lUrlString));
            emit sFinished(lDownloadID, mDownloadData[lDownloadID].mFileContents);
          }
          else {
            QFile lTargetFile(lFilePath);
            if(lTargetFile.open(QIODevice::WriteOnly)) {
              lTargetFile.write(mDownloadData[lDownloadID].mFileContents);
              lTargetFile.flush();
              lTargetFile.close();
              if(mDownloadData[lDownloadID].mNotifyIfSuccessfull)
                emit sLog(ELogLevel::eLogInfo, ELogMessageType::eResourceManagement, tr("File '%1' downloaded and saved successfully.").arg(lUrlString));
              emit sFinished(lDownloadID, QByteArray());
            }
            else {
              emit sLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, tr("File '%1' couldn't be written to disk.").arg(lUrlString));
              emit sError(lDownloadID);
            }
          }
        }
        mDownloadData[lDownloadID].pTimeoutTimer->deleteLater();
        rNetworkReply->deleteLater();
        mDownloadData.remove(lDownloadID);
      } );

    connect(rNetworkReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), rNetworkReply, [lDownloadID, lUrlString, rNetworkReply, this] (QNetworkReply::NetworkError /*lCode*/) {
        emit sLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, tr("File '%1' couldn't be downloaded. Error: '%2'.").arg(lUrlString).arg(rNetworkReply->errorString()));
        emit sError(lDownloadID);
      } );

    connect(rDownloadTimer, &QTimer::timeout, rDownloadTimer, [lFilePath, lDownloadID, lUrlString, rDownloadTimer, this] () {
        if(mDownloadData[lDownloadID].mRetryTimes == cCount_RetryTimes) {
          rDownloadTimer->stop();
          mDownloadData[lDownloadID].pNetworkReply->abort();
          emit sLog(ELogLevel::eLogCritical, ELogMessageType::eResourceManagement, tr("File '%1' couldn't be downloaded succesfully after '%2' retries.").arg(lUrlString).arg(cCount_RetryTimes));
          emit sError(lDownloadID);
        }
        else {
          rDownloadTimer->stop();
          mDownloadData[lDownloadID].pNetworkReply->abort();
          emit sLog(ELogLevel::eLogWarning, ELogMessageType::eResourceManagement, tr("File '%1' download timeout, retrying ...").arg(lUrlString));
          metaObject()->invokeMethod(this, "fDownload", Qt::QueuedConnection, Q_ARG(QUrl, QUrl(lUrlString)), Q_ARG(QString, lFilePath), Q_ARG(bool, mDownloadData[lDownloadID].mNotifyIfSuccessfull), Q_ARG(QString, lDownloadID));
        }
      } );

    rDownloadTimer->start(cTime_DownloadTimeout * 1000);
  }
}
