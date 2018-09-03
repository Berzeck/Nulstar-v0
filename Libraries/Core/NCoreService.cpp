#include "NCoreService.h"

namespace NulstarNS {
  NCoreService::NCoreService(ELogLevel lLogLevel, QList<QNetworkAddressEntry> lAllowedNetworks, QObject *rParent)
              : QObject(rParent), mLogLevel(lLogLevel), mAllowedNetworks(lAllowedNetworks) {

  }

  void NCoreService::fConnectToServiceManager(const QUrl& lUrl) {
    connect(&mWebSocket, &QWebSocket::connected, this, &NCoreService::fOnConnected);
    connect(&mWebSocket, &QWebSocket::disconnected, this, &NCoreService::sClosed);
    mWebSocket.open(lUrl);
  }

  void NCoreService::fOnConnected() {
    connect(&mWebSocket, &QWebSocket::textMessageReceived, this, &NCoreService::fOnTextMessageReceived);
    mWebSocket.sendTextMessage(QStringLiteral("Sending Roles!"));
setProperty("CU", QString("111"));
  }

  void NCoreService::fOnTextMessageReceived(const QString &lTextMessage) {
    mWebSocket.sendTextMessage(QString("Receieved:\n%1").arg(lTextMessage));
  }
}
