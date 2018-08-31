#include <QCommandLineParser>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include "NServiceManagerController.h"

int main(int argc, char *argv[])
{
  QCoreApplication lApp(argc, argv);
  QString lAppName(QString(APP_NAME).replace("_"," "));
  lApp.setApplicationName(lAppName);
  lApp.setApplicationVersion(APP_VERSION);
  lApp.setOrganizationDomain(QStringLiteral("nulstar.com"));
  lApp.setOrganizationName(QStringLiteral("Nulstar"));

  QCommandLineParser lParser;
  lParser.setApplicationDescription(lAppName);
  lParser.addHelpOption();
  lParser.addVersionOption();
  lParser.addOptions({
    {{"d", "loglevel"} , QStringLiteral("Log Level [1-5]."), QStringLiteral("loglevel")},
    {{"s", "sectype"}, QStringLiteral("Security Type [0-1]."), QStringLiteral("sectype")},
    {{"m", "commport"}, QStringLiteral("Communication Port."), QStringLiteral("commport")},
  });
  lParser.process(lApp);
  if(!lParser.isSet("loglevel") || lParser.value("loglevel").toUShort() < 1 || lParser.value("loglevel").toUShort() > 5) {
    fputs(qPrintable(QString("Wrong debug level! [1-5]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 1;
  }
  if(!lParser.isSet("sectype") || (lParser.value("sectype").toUShort() != 0 && lParser.value("sectype").toUShort() != 1)) {
    fputs(qPrintable(QString("Wrong security type! [0-1]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 2;
  }
  if(!lParser.isSet("commport")) {
    fputs(qPrintable(QString("Communication port not set!\n\n%1\n").arg(lParser.helpText())), stderr);
    return 5;
  }
  using NulstarNS::NServiceManagerController;
  NServiceManagerController lController(static_cast<QWebSocketServer::SslMode> (lParser.value("sectype").toUInt()), static_cast<NServiceManagerController::ELogLevel> (lParser.value("loglevel").toUInt()));
  lController.fControlCommServer(NServiceManagerController::EServiceAction::eStartService, static_cast<quint16> (lParser.value("commport").toUInt()));
  return lApp.exec();
}
