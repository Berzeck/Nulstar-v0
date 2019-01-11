//
// Created by daviyang35 on 2019-01-10.
//
#include <QtCore>
#include <QtWebSockets>
#include "AppVersion.h"
#include "NUpdateController.h"

int main(int argc, char *argv[]) {
  QCoreApplication lApp(argc, argv);
  lApp.setApplicationName(APP_NAME);
  lApp.setApplicationVersion(APP_VERSION);
  lApp.setOrganizationDomain(APP_DOMAIN);
  lApp.setOrganizationName(APP_DOMAIN);

  QList <QNetworkAddressEntry> lAllowedNetworks;
  QCommandLineParser lParser;
  QString lServiceManagerUrl;
  lParser.setApplicationDescription(APP_NAME);
  lParser.addHelpOption();
  lParser.addVersionOption();
  lParser.addOptions({
                         {{"d", "loglevel"}, QStringLiteral("Log Level [1-5]."), QStringLiteral("loglevel")},
                         {{"s", "sslmode"}, QStringLiteral("Security Type [0-1]."), QStringLiteral("sslmode")},
                         {{"a", "adminport"}, QStringLiteral("Admin Port."), QStringLiteral("adminport")},
                         {{"c", "clientport"}, QStringLiteral("Client Port."), QStringLiteral("clientport")},
                         {{"m", "commport"}, QStringLiteral("Communication Port."), QStringLiteral("commport")},
                         {{"n", "allowednetworks"}, QStringLiteral("Allowed Networks."),
                          QStringLiteral("allowednetworks")},
                         {{"i", "managerurl"}, QStringLiteral("Service manager URL."), QStringLiteral("managerurl")},
                     });
  lParser.process(lApp);
  if (!lParser.isSet("loglevel") || lParser.value("loglevel").toUShort() < 1
      || lParser.value("loglevel").toUShort() > 5) {
    fputs(qPrintable(QString("Wrong debug level! [1-5]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 1;
  }
  QString lSslModeStr(lParser.value("sslmode"));
  if (!lParser.isSet("sslmode") || (lSslModeStr.toUShort() != 0 && lSslModeStr.toUShort() != 1)) {
    fputs(qPrintable(QString("Wrong security type! [0-1]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 2;
  }
  if (!lParser.isSet("adminport")) {
    fputs(qPrintable(QString("Admin port not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 3;
  }
  if (!lParser.isSet("clientport")) {
    fputs(qPrintable(QString("Client port not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 4;
  }
  if (!lParser.isSet("commport")) {
    fputs(qPrintable(QString("Communication port not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 5;
  }
  QWebSocketServer::SslMode lSslMode = QWebSocketServer::SslMode::NonSecureMode;
  if (lParser.isSet("managerurl")) {
    lServiceManagerUrl = lParser.value("managerurl");
    if (lSslModeStr.toUInt() == 0)
      lServiceManagerUrl.prepend("ws://");
    if (lSslModeStr.toUInt() == 1) {
      lServiceManagerUrl.prepend("wss://");
      lSslMode = QWebSocketServer::SslMode::SecureMode;
    }
  }
  if (lParser.isSet("allowednetworks")) {
    QStringList lNetworks(lParser.value("allowednetworks").split(","));
    for (const QString &lNetwork : lNetworks) {
      QStringList lParams(lNetwork.split("/"));
      QNetworkAddressEntry lNetworkAddress;
      lNetworkAddress.setIp(QHostAddress(lParams.at(0).simplified()));
      if (lParams.size() > 1)
        lNetworkAddress.setPrefixLength(lParams.at(1).simplified().toInt());
      lAllowedNetworks << lNetworkAddress;
    }
  }
  NulstarNS::NUpdateController lController(lSslMode,
                                               static_cast<NulstarNS::NUpdateController::ELogLevel> (lParser
                                                   .value("loglevel").toUInt()),
                                               QUrl(lServiceManagerUrl),
                                               lAllowedNetworks,
                                               lParser.value("commport").toUShort(),
                                               lParser.value("adminport").toUShort(),
                                               lParser.value("clientport").toUShort(),
                                               QHostAddress::Any);
//  lController.fControlWebServer(QString(),
//                                NulstarNS::NConnectionController::EServiceAction::eStartService);  // Start all web sockets servers
  return lApp.exec();
}