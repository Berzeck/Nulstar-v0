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
#include "NConnectionController.h"

const QString lOrganizationName = "Nulstar";

int main(int argc, char *argv[])
{    
  QCoreApplication lApp(argc, argv);
  QString lAppName(QString(APP_NAME).replace("_"," "));
  lApp.setApplicationName(lAppName);
  lApp.setApplicationVersion(APP_VERSION);
  lApp.setOrganizationDomain(APP_DOMAIN);
  lApp.setOrganizationName(lOrganizationName);

  QList<QNetworkAddressEntry> lAllowedNetworks;
  QCommandLineParser lParser;
  QString lServiceManagerUrl;
  unsigned short lSslMode = 0;
  lParser.setApplicationDescription(lAppName);
  lParser.addHelpOption();
  lParser.addVersionOption();
  lParser.addOptions({
    {{"d", "loglevel"} , QStringLiteral("Log Level [1-5]."), QStringLiteral("loglevel")},
    {{"s", "sslmode"}, QStringLiteral("Security Type [0-1]."), QStringLiteral("sslmode")},
    {{"a", "adminport"}, QStringLiteral("Admin Port."), QStringLiteral("adminport")},
    {{"c", "clientport"}, QStringLiteral("Client Port."), QStringLiteral("clientport")},
    {{"m", "commport"}, QStringLiteral("Communication Port."), QStringLiteral("commport")},
    {{"n", "allowednetworks"}, QStringLiteral("Allowed Networks."), QStringLiteral("allowednetworks")},
    {{"i", "managerurl"}, QStringLiteral("Service manager URL."), QStringLiteral("managerurl")},
  });
  lParser.process(lApp);
  if(!lParser.isSet("loglevel") || lParser.value("loglevel").toUShort() < 1 || lParser.value("loglevel").toUShort() > 5) {
    fputs(qPrintable(QString("Wrong debug level! [1-5]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 1;
  }
  if(!lParser.isSet("sslmode") || (lParser.value("sslmode").toUShort() != 0 && lParser.value("sslmode").toUShort() != 1)) {
    fputs(qPrintable(QString("Wrong security type! [0-1]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 2;
  }
  lSslMode = lParser.value("sslmode").toUShort();
  if(!lParser.isSet("adminport")) {
    fputs(qPrintable(QString("Admin port not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 3;
  }
  if(!lParser.isSet("clientport")) {
    fputs(qPrintable(QString("Client port not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 4;
  }
  if(!lParser.isSet("commport")) {
    fputs(qPrintable(QString("Communication port not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 5;
  }
  if(lParser.isSet("managerurl")) {
    lServiceManagerUrl = lParser.value("managerurl");
    if(lSslMode == 0) lServiceManagerUrl.prepend("ws://");
    if(lSslMode == 1) lServiceManagerUrl.prepend("wss://");
  }
qDebug(lServiceManagerUrl.toLatin1());
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
  NulstarNS::NConnectionController lController(static_cast<QWebSocketServer::SslMode> (lParser.value("sslmode").toUInt()), static_cast<NulstarNS::NConnectionController::ELogLevel> (lParser.value("loglevel").toUInt()), QUrl(lServiceManagerUrl),
                                   lAllowedNetworks, lParser.value("commport").toUShort(), QHostAddress::Any);
  lController.fControlWebServer(QString(), NulstarNS::NConnectionController::EServiceAction::eStartService);  // Start all web sockets servers
  return lApp.exec();
}
