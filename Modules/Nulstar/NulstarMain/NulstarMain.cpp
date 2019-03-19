#include <QCommandLineParser>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <NCoreConstants.h>
#include <NSignalWatcher.h>

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
  lController.startmodule(APP_DOMAIN, NulstarNS::cServiceManagerName);
  lController.startallmodules();
  lController.fConnectToServiceManager(NulstarNS::cRetryInterval);

#ifdef Q_OS_UNIX
  NSignalWatcher lSignalWatcher;
  lSignalWatcher.watchForSignal(SIGINT);
  lSignalWatcher.watchForSignal(SIGTERM);
  QObject::connect(&lSignalWatcher, SIGNAL(unixSignal(int)), &lApp, SLOT(quit()));
#endif

  return lApp.exec();
}

