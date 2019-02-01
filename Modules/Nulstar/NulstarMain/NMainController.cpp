#include <QCoreApplication>
#include <QHostAddress>
#include <QProcess>
#include <QWebSocketServer>

#include "NMainController.h"

const int cProcessStartMaxSeconds = 5;
const QString cComponentsDirectory("MainModules");
const QString cMainControllerIPParameter("MainControllerIP");
const QString cManageParameter("Managed");
const QString cAllowedNetworks("AllowedNetworks");
const QString cCommPortParameter("CommPort");
const QString cLogLevelParameter("LogLevel");
const QString cSslModeParameter("SslMode");

namespace NulstarNS {
  NMainController::NMainController()
                 : NCoreService() {

    NModuleInfo lControllerInfo = mModuleManager.fModuleInfo(APP_DOMAIN, APP_NAME);

    QList<QNetworkAddressEntry> lAllowedNetworks;
    QHostAddress::SpecialAddress lBindAddress = QHostAddress::Any;
    QHostAddress lHostAddress;
    lHostAddress.setAddress(lControllerInfo.fParameterValue(cMainControllerIPParameter));

    QString lCommunicationPort(lControllerInfo.fParameterValue(cCommPortParameter));
    QString lLogLevel(lControllerInfo.fParameterValue(cLogLevelParameter));
    QString lSslModeStr(lControllerInfo.fParameterValue(cSslModeParameter));
    QStringList lAllowedNetworksStrings(lControllerInfo.fParameterValue(cAllowedNetworks).split(","));
    for(const QString& lNetwork : lAllowedNetworksStrings) {
      QStringList lParams(lNetwork.split("/"));
      QNetworkAddressEntry lNetworkAddress;
      lNetworkAddress.setIp(QHostAddress(lParams.at(0).simplified()));
      if(lParams.size() > 1)
        lNetworkAddress.setPrefixLength(lParams.at(1).simplified().toInt());
      lAllowedNetworks << lNetworkAddress;
    }

    NModuleInfo lServiceManagerInfo = mModuleManager.fModuleInfo(APP_DOMAIN, cServiceManager);
    QString lServiceManagerPort = lServiceManagerInfo.fParameterValue("CommPort");
    QString lServiceManagerUrl = QHostAddress(QHostAddress::LocalHost).toString();
    lServiceManagerUrl.append(QString(":%1").arg(lServiceManagerPort));

    QWebSocketServer::SslMode lSslMode = QWebSocketServer::SslMode::NonSecureMode;
    if(lSslModeStr.toUShort() == 0)
      lServiceManagerUrl.prepend("ws://");
    if(lSslModeStr.toUShort() == 1) {
      lServiceManagerUrl.prepend("wss://");
      lSslMode = QWebSocketServer::SslMode::SecureMode;
    }

    fSetSslMode(lSslMode);
    fSetLogLevel(static_cast<NulstarNS::NMainController::ELogLevel>(lLogLevel.toUInt()));
    fSetServiceManagerUrl(QUrl(lServiceManagerUrl));
    fSetAllowedNetworks(lAllowedNetworks);
    fAddWebSocketServer(lCommunicationPort.toUShort(), lBindAddress, fName());
  }

  bool NMainController::fStartModule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion, bool fRestartIfRunning) {
    QString lEffectiveModuleVersion = lModuleVersion.isEmpty() ? mModuleManager.fGetModuleLastVersion(lModuleNamespace, lModuleName) : lModuleVersion;
    if(lEffectiveModuleVersion.isEmpty()) {
      qDebug("Module '%s' does not exist!", lModuleName.toStdString().data());
      return false;
    }
    QString lProcessIndex(QString("%1%2%3%4%5").arg(lModuleNamespace).arg(cIndexSeparator).arg(lModuleName).arg(cIndexSeparator).arg(lEffectiveModuleVersion));
    if(mModulesRunning.contains(lProcessIndex)) {
      if(fRestartIfRunning) {
        fStopModule(lModuleNamespace, lModuleName, lEffectiveModuleVersion);
      }
      else
        return true;
    }

    if((lModuleName == APP_NAME) && (lModuleNamespace == APP_DOMAIN)) {
      qDebug("Module '%s' can't be managed from this interface!", lModuleName.toStdString().data());
      return false;
    }
    QProcess* lModuleProcess = new QProcess(this);
    NModuleInfo lModuleInfo = mModuleManager.fModuleInfo(lModuleNamespace, lModuleName, lEffectiveModuleVersion);
/*  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LD_LIBRARY_PATH", "/home/Berzeck/Applications/Nulstar/Debug/Libraries/CPP/Nulstar/0.1.0"); // Add an environment variable
  lModuleProcess->setProcessEnvironment(env);*/

    lModuleProcess->start(lModuleInfo.fModuleAppPath(), lModuleInfo.fFormattedParameters());
    if(lModuleProcess->waitForStarted(cProcessStartMaxSeconds * 1000)) {
      mModulesRunning.insert(lProcessIndex, lModuleProcess);
      return true;
    }
    qDebug("Module '%s' and version '%s' could not start successfully!", lModuleName.toStdString().data(), lEffectiveModuleVersion.toStdString().data());
    lModuleProcess->deleteLater();

    return false;
  }

  bool NMainController::fStartAllModules(const QString& lModuleNamespace, bool fRestartIfRunning) {
    bool lAllModulesSuccessfullyStarted = true;
    QList<NModuleInfo> lModuleInfoList(mModuleManager.fModuleInfoList());
    for(const NModuleInfo& lModuleInfo : lModuleInfoList) {
      QString lCurrentModuleNamespace(lModuleInfo.fModuleNamespace());
      if(lModuleNamespace.isEmpty() || (lModuleNamespace == lCurrentModuleNamespace)) {
        QString lModuleName(lModuleInfo.fModuleName());
        QString lModuleVersion(lModuleInfo.fModuleVersion());
        if((lModuleName == APP_NAME) && (lCurrentModuleNamespace == APP_DOMAIN))
          continue;
        lAllModulesSuccessfullyStarted = lAllModulesSuccessfullyStarted && fStartModule(lCurrentModuleNamespace, lModuleName, lModuleVersion, fRestartIfRunning);
      }
    }
    return lAllModulesSuccessfullyStarted;
  }

  bool NMainController::fStopModule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion) {
    QString lEffectiveModuleVersion = lModuleVersion.isEmpty() ? mModuleManager.fGetModuleLastVersion(lModuleNamespace, lModuleName) : lModuleVersion;
    if(lEffectiveModuleVersion.isEmpty()) {
      qDebug("Module '%s' does not exist!", lModuleName.toStdString().data());
      return false;
    }
    if((lModuleName == APP_NAME) && (lModuleNamespace == APP_DOMAIN)) {
      qDebug("Module '%s' can't be managed fron this interface!", lModuleName.toStdString().data());
      return false;
    }

    QString lProcessIndex(QString("%1%2%3%4%5").arg(lModuleNamespace).arg(cIndexSeparator).arg(lModuleName).arg(cIndexSeparator).arg(lEffectiveModuleVersion));
    if(mModulesRunning.contains(lProcessIndex)) {
      QProcess* lModuleProcess = mModulesRunning.value(lProcessIndex);
      if(lModuleProcess) {
        lModuleProcess->close();
        mModulesRunning.remove(lProcessIndex);
        lModuleProcess->deleteLater();

        return true;
      }
    }

    qDebug("Module '%s' with version '%s' is not running!", lModuleName.toStdString().data(), lEffectiveModuleVersion.toStdString().data());
    return false;
  }

  bool NMainController::fStopAllModules(const QString &lModuleNamespace) {
    bool lAllModulesSuccessfullyStoped = true;
    QMapIterator<QString, QProcess*> i1(mModulesRunning);
    while(i1.hasNext()) {
      i1.next();
      QString lCurrentModuleNamespace(i1.key().split("--").at(0));
      if(lModuleNamespace.isEmpty() || (lModuleNamespace == lCurrentModuleNamespace)) {
        QString lModuleName(i1.key().split("--").at(1));
        QString lModuleVersion(i1.key().split("--").at(2));
        if((lModuleName == APP_NAME) && (lCurrentModuleNamespace == APP_DOMAIN))
          continue;
        lAllModulesSuccessfullyStoped = lAllModulesSuccessfullyStoped && fStopModule(lCurrentModuleNamespace, lModuleName, lModuleVersion);
      }
    }
    return lAllModulesSuccessfullyStoped;
  }

  void NMainController::fShutdownSystem() {
    fStopAllModules();
    qApp->quit();
  }
}
