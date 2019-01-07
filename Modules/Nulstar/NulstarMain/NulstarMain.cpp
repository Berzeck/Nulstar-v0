#include <QCommandLineParser>
#include <QCoreApplication>
#include <QString>
#include <QStringList>

#include "NMainController.h"

int main(int argc, char* argv[])
{
  QCoreApplication lApp(argc, argv);
  lApp.setApplicationName(APP_NAME);
  lApp.setApplicationVersion(APP_VERSION);
  lApp.setOrganizationDomain(QStringLiteral(APP_DOMAIN));
  lApp.setOrganizationName(QStringLiteral(APP_DOMAIN));

  QCommandLineParser lParser;
  lParser.setApplicationDescription(APP_NAME);
  lParser.addHelpOption();
  lParser.addVersionOption();
  lParser.process(lApp);

  NulstarNS::NMainController lController;
  lController.fControlWebServer(QString(), NulstarNS::NMainController::EServiceAction::eStartService); // Start all web sockets servers
  lController.fStartModule(APP_DOMAIN, cServiceManager);
  lController.fStartAllModules();
  return lApp.exec();
}
