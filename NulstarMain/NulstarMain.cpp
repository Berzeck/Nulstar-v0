#include <QCoreApplication>
#include <QHostAddress>
#include <QCommandLineParser>
#include <QPair>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QUrl>
#include "NMainController.h"

const QString lConstantsFile("Constants.ncf");
const QString lControllerName("ServiceController");
const QString lManageParameter("Managed");
const QString lServiceManagerUrlParameter("managerurl");

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
  lParser.process(lApp);

  QString lConfigFile;
  QSettings lConstants(lConstantsFile, QSettings::IniFormat);
  lConstants.beginGroup(lControllerName);
  lConfigFile = lConstants.value(QString("ConfigFile")).toString();
  lConstants.endGroup();

  QList<QNetworkAddressEntry> lAllowedNetworks;
  QHostAddress lHostAddress;
  QHostAddress::SpecialAddress lBindAddress;
  QString lCommunicationPort, lLogLevel, lSslMode;
  QSettings lSettings(lConfigFile, QSettings::IniFormat);
  QStringList  lNetworks, lGroups(lSettings.childGroups());
  lSettings.beginGroup(lControllerName);
  lCommunicationPort = lSettings.value(QString("CommunicationPort")).toString();
  lLogLevel = lSettings.value(QString("LogLevel")).toString();
  lHostAddress.setAddress(lSettings.value(QString("MainControllerIP")).toString());
  lSslMode = lSettings.value(QString("SslMode")).toString();
  lNetworks = lSettings.value(QString("AllowedNetworks")).toString().split(",");
  lSettings.endGroup();

  for(const QString& lNetwork : lNetworks) {
    QStringList lParams(lNetwork.split("/"));
    QNetworkAddressEntry lNetworkAddress;
    lNetworkAddress.setIp(QHostAddress(lParams.at(0).simplified()));
    if(lParams.size() > 1) lNetworkAddress.setPrefixLength(lParams.at(1).simplified().toInt());
    lAllowedNetworks << lNetworkAddress;
  }
  if(lHostAddress.isNull() || lHostAddress.isLoopback()) lBindAddress = QHostAddress::LocalHost;
  else lBindAddress = QHostAddress::Any;
  NulstarNS::NMainController lController(static_cast<QWebSocketServer::SslMode> (lSslMode.toUInt()), static_cast<NulstarNS::NMainController::ELogLevel> (lLogLevel.toUInt()), QUrl(QHostAddress(QHostAddress::LocalHost).toString()), lAllowedNetworks, lCommunicationPort.toUShort(), lBindAddress);
  lController.fControlWebServer(QString(), NulstarNS::NMainController::EServiceAction::eStartService);  // Start all web sockets servers

  for(const QString& lGroup : lGroups) {
    lSettings.beginGroup(lGroup);
    if(lGroup != "ServiceController") {
      QList<QPair<QString, QString> > lParameters;
      QStringList lKeys(lSettings.childKeys());
      bool lManaged(lSettings.value(lManageParameter, false).toBool());
      if(lManaged) {
        for(const QString& lKey : lKeys) {
          if(lKey != lManageParameter) {
            QPair<QString, QString> lParameter;
            lParameter.first = lKey.toLower();
            lParameter.second = lSettings.value(lKey).toString();
            lParameters << lParameter;
          }
        }
        if(lGroup != "ServiceManager") {
          QPair<QString, QString> lServiceManagerUrl;
          QSettings lServiceManagerSettings(lConfigFile, QSettings::IniFormat);
          lServiceManagerSettings.beginGroup("ServiceManager");
          QString lPort = lServiceManagerSettings.value("CommPort").toString();
          lServiceManagerUrl.first = lServiceManagerUrlParameter;
          lServiceManagerUrl.second = QString("%1:%2").arg(lHostAddress.toString()).arg(lPort);
          lServiceManagerSettings.endGroup();
          lParameters << lServiceManagerUrl;
        }
        lController.fSetComponent(lGroup, lParameters);
      }      
    }
    lSettings.endGroup();
  }  
  return lApp.exec();
}
