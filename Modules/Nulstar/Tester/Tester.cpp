#include <QCommandLineParser>
#include <QCoreApplication>
#include <QHostAddress>
#include <QList>
#include <QNetworkAddressEntry>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QUrl>
#include <Core.h>

#include "NTesterController.h"

int main(int argc, char *argv[])
{
  QCoreApplication lApp(argc, argv);
  lApp.setApplicationName(APP_NAME);
  lApp.setApplicationVersion(APP_VERSION);
  lApp.setOrganizationDomain(APP_DOMAIN);
  lApp.setOrganizationName(APP_DOMAIN);

  QList<QNetworkAddressEntry> lAllowedNetworks;
  QCommandLineParser lParser;
  lParser.setApplicationDescription(APP_NAME);
  lParser.addHelpOption();
  lParser.addVersionOption();
  lParser.addOptions({
    {{"d", "loglevel"} , QStringLiteral("Log Level [1-5]."), QStringLiteral("loglevel")},
    {{"s", "sslmode"}, QStringLiteral("Security Type [0-1]."), QStringLiteral("sslmode")},
    {{"m", "commport"}, QStringLiteral("Communication Port."), QStringLiteral("commport")},
    {{"n", "allowednetworks"}, QStringLiteral("Allowed Networks."), QStringLiteral("allowednetworks")},
    {{"i", "managerurl"}, QStringLiteral("Service manager URL."), QStringLiteral("managerurl")},
  });
  lParser.process(lApp);
  if(!lParser.isSet("loglevel") || lParser.value("loglevel").toUShort() < 1 || lParser.value("loglevel").toUShort() > 5) {
    fputs(qPrintable(QString("Wrong debug level! [1-5]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 1;
  }
  QString lSslModeStr(lParser.value("sslmode"));
  if(!lParser.isSet("sslmode") || (lSslModeStr.toUShort() != 0 && lSslModeStr.toUShort() != 1)) {
    fputs(qPrintable(QString("Wrong security type! [0-1]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 2;
  }
  if(!lParser.isSet("managerurl")) {
    fputs(qPrintable(QString("Service Manager URL not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 3;
  }
  if(!lParser.isSet("commport")) {
    fputs(qPrintable(QString("Communication port not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 5;
  }
  if(lParser.isSet("allowednetworks")) {
    QStringList lNetworks(lParser.value("allowednetworks").split(","));
    for(const QString& lNetwork : lNetworks) {
      QStringList lParams(lNetwork.split("/"));
      QNetworkAddressEntry lNetworkAddress;
      lNetworkAddress.setIp(QHostAddress(lParams.at(0).simplified()));
      if(lParams.size() > 1) lNetworkAddress.setPrefixLength(lParams.at(1).simplified().toInt());
      lAllowedNetworks << lNetworkAddress;
    }
  }
  QString lServiceManagerUrl = lParser.value("managerurl");
  QWebSocketServer::SslMode lSslMode = QWebSocketServer::SslMode::NonSecureMode;
  if(lSslModeStr.toUInt() == 0) {
      lServiceManagerUrl.prepend("ws://");
  }
  else if(lSslModeStr.toUInt() == 1) {
      lServiceManagerUrl.prepend("wss://");
      lSslMode = QWebSocketServer::SslMode::SecureMode;
  }
  const int lRetryInterval = 5; // Seconds
  NulstarNS::NTesterController lController(lSslMode, static_cast<NulstarNS::NTesterController::ELogLevel> (lParser.value("loglevel").toUInt()),
                                           QUrl(lServiceManagerUrl), lAllowedNetworks, lParser.value("commport").toUShort(), QHostAddress::Any);
  lController.fControlWebServer(QString(), NulstarNS::NTesterController::EServiceAction::eStartService);
  lController.fConnectToServiceManager(lRetryInterval);
  return lApp.exec();
}
