#include <QCoreApplication>
#include <QEventLoop>
#include <QHostAddress>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QWebSocketServer>

#include "NStatsManagerController.h"

namespace NulstarNS {
    NStatsManagerController::NStatsManagerController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress &lIP, const QUrl &lServiceManagerUrl,
                                                     const QString &lMainPath, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
      : NCoreService(lSslMode, lLogLevel, lIP, lServiceManagerUrl, lMainPath, lAllowedNetworks, rParent) {

    if(lCommPort)
      fAddWebSocketServer(lCommPort, lBindAddress);

    fFillMethodMetadata();
  }

  void NStatsManagerController::fFillMethodMetadata() {
    fAddMethodFunctionDescription(QStringLiteral("GetCPUInfo"), tr("Query information about CPU"));
    fAddMethodFunctionDescription(QStringLiteral("GetRAMInfo"), tr("Query information about RAM memory"));

    fAddMethodMinEventAndMinPeriod(QStringLiteral("GetCPUInfo"), QString("0,3"));
    fAddMethodMinEventAndMinPeriod(QStringLiteral("GetRAMInfo"), QString("0,3"));
  }

  QVariantMap NStatsManagerController::fApiRoles() const {
    QVariantMap lApiRolesMap;
    QStringList lApiRoles(QString(APP_ROLES).split(":"));
    for(QString& lApiRole : lApiRoles) {
      lApiRole.remove(' ');
      QString lRole = lApiRole.split("[").at(0);
      QStringList lSupportedVersions = lApiRole.split("[").at(1).split("]").at(0).split(",");
      lApiRolesMap[lRole] = lSupportedVersions;
    }
    return lApiRolesMap;
  }

  void NStatsManagerController::GetCPUInfo() {

  }

  void NStatsManagerController::GetRAMInfo() {

  }
}
