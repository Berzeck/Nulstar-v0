#include <QCommandLineParser>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include "NConnectionController.h"

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
    {{"d", "debug"} , QStringLiteral("Debug Level [1-5]."), QStringLiteral("debug")},
    {{"s", "sectype"}, QStringLiteral("Security Type [0-1]."), QStringLiteral("sectype")},
    {{"a", "adminport"}, QStringLiteral("Admin Port."), QStringLiteral("adminport")},
    {{"c", "clientport"}, QStringLiteral("Client Port."), QStringLiteral("clientport")},
    {{"m", "commport"}, QStringLiteral("Communication Port."), QStringLiteral("commport")},
  });
  lParser.process(lApp);
  if(!lParser.isSet("debug") || lParser.value("debug").toUShort() < 1 || lParser.value("debug").toUShort() > 5) {
    fputs(qPrintable(QString("Wrong debug level! [1-5]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 1;
  }
  if(!lParser.isSet("sectype") || (lParser.value("sectype").toUShort() != 0 && lParser.value("sectype").toUShort() != 1)) {
    fputs(qPrintable(QString("Wrong security type! [0-1]\n\n%1\n").arg(lParser.helpText())), stderr);
    return 2;
  }
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
  return lApp.exec();
}
