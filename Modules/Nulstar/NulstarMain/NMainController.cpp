#include <QCoreApplication>
#include <QEventLoop>
#include <QHostAddress>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QWebSocketServer>

#include "NMainController.h"

namespace NulstarNS {
  const int cProcessStartMaxSeconds = 5;
  const int cTimePeriod_ExtraDelayMSecs = 500;
  const QString cComponentsDirectory("MainModules");
  const QString cMainControllerIPParameter("MainControllerIP");
  const QString cManageParameter("Managed");
  const QString cAllowedNetworks("AllowedNetworks");
  const QString cCommPortParameter("CommPort");
  const QString cLogLevelParameter("LogLevel");
  const QString cIPParameter("IP");
  const QString cSslModeParameter("SslMode");
  const QString cParameter_ManagerURL("managerurl");

  NMainController::NMainController()
                 : NCoreService() {

    NModuleInfo lControllerInfo = mModuleManager.fModuleInfo(APP_DOMAIN, APP_NAME);
    QList<QNetworkAddressEntry> lAllowedNetworks;
    QHostAddress::SpecialAddress lBindAddress = QHostAddress::AnyIPv4;
    QHostAddress lHostAddress;
    lHostAddress.setAddress(lControllerInfo.fParameterValue(cMainControllerIPParameter));

    QString lCommunicationPort(lControllerInfo.fParameterValue(cCommPortParameter));
    QString lLogLevel(lControllerInfo.fParameterValue(cLogLevelParameter));
    QString lSslModeStr(lControllerInfo.fParameterValue(cSslModeParameter));
    QString lIP(lControllerInfo.fParameterValue(cIPParameter));
    QStringList lAllowedNetworksStrings(lControllerInfo.fParameterValue(cAllowedNetworks).split(","));
    for(const QString& lNetwork : lAllowedNetworksStrings) {
      QStringList lParams(lNetwork.split("/"));
      QNetworkAddressEntry lNetworkAddress;
      lNetworkAddress.setIp(QHostAddress(lParams.at(0).simplified()));
      if(lParams.size() > 1)
        lNetworkAddress.setPrefixLength(lParams.at(1).simplified().toInt());
      lAllowedNetworks << lNetworkAddress;
    }

    NModuleInfo lServiceManagerInfo = mModuleManager.fModuleInfo(APP_DOMAIN, cServiceManagerName);
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
    fSetLogLevel(static_cast<NulstarNS::ELogLevel>(lLogLevel.toUInt()));
    fSetServiceManagerUrl(QUrl(lServiceManagerUrl));
    fSetAllowedNetworks(lAllowedNetworks);
    fSetHost(QHostAddress(lIP));
    fAddWebSocketServer(lCommunicationPort.toUShort(), lBindAddress);

    fFillMethodMetadata();
  }

  QVariantMap NMainController::fApiRoles() const {
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

  quint8 NMainController::scanmanagedmodules() {
    return mModuleManager.fLoadModulesInfo();
  }

  bool NMainController::startmodule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion, bool fRestartIfRunning) {
    QString lEffectiveModuleVersion = lModuleVersion.isEmpty() ? mModuleManager.fGetModuleLastVersion(lModuleNamespace, lModuleName) : lModuleVersion;
    if(lEffectiveModuleVersion.isEmpty()) {
      qDebug("Module '%s' does not exist!", lModuleName.toStdString().data());
      return false;
    }
    QString lProcessIndex(QString("%1%2%3%4%5").arg(lModuleNamespace).arg(cIndexSeparator).arg(lModuleName).arg(cIndexSeparator).arg(lEffectiveModuleVersion));
    if(mModulesRunning.contains(lProcessIndex)) {
      if(fRestartIfRunning) {
        stopmodule(lModuleNamespace, lModuleName, lEffectiveModuleVersion);
      }
      else
        return true;
    }

    if((lModuleName == APP_NAME) && (lModuleNamespace == APP_DOMAIN)) {
      qDebug("Module '%s' can't be managed from this interface!", lModuleName.toStdString().data());
      return false;
    }
    QProcess* rModuleProcess = new QProcess(this);
    NModuleInfo lModuleInfo = mModuleManager.fModuleInfo(lModuleNamespace, lModuleName, lEffectiveModuleVersion);
/*  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LD_LIBRARY_PATH", "/home/Berzeck/Applications/Nulstar/Debug/Libraries/CPP/Nulstar/0.1.0"); // Add an environment variable
  rModuleProcess->setProcessEnvironment(env);*/
    QStringList lFormattedParameters(lModuleInfo.fFormattedParameters());
    if(lModuleInfo.fModuleName() != cServiceManagerName)
      lFormattedParameters << QString("--%1").arg(cParameter_ManagerURL) << fServiceManagerUrl().toString();
    rModuleProcess->start(lModuleInfo.fModuleAppPath(), lFormattedParameters);
    if(rModuleProcess->waitForStarted(cProcessStartMaxSeconds * 1000)) {
      mModulesRunning.insert(lProcessIndex, rModuleProcess);
      QEventLoop lExtraDelay;  // So components have enough time to open the websocket servers
      QTimer lDelayTimer;
      connect(&lDelayTimer, &QTimer::timeout, &lExtraDelay, &QEventLoop::quit);
      lDelayTimer.start(cTimePeriod_ExtraDelayMSecs);
      lExtraDelay.exec();
      return true;
    }
    qDebug("Module '%s' and version '%s' could not start successfully!", lModuleName.toStdString().data(), lEffectiveModuleVersion.toStdString().data());
    rModuleProcess->deleteLater();
    return false;
  }

  bool NMainController::startallmodules(const QString& lModuleNamespace, bool fRestartIfRunning) {
    bool lAllModulesSuccessfullyStarted = true;
    QList<NModuleInfo> lModuleInfoList(mModuleManager.fModuleInfoList());
    for(const NModuleInfo& lModuleInfo : lModuleInfoList) {
      QString lCurrentModuleNamespace(lModuleInfo.fModuleNamespace());
      if(lModuleNamespace.isEmpty() || (lModuleNamespace == lCurrentModuleNamespace)) {
        QString lModuleName(lModuleInfo.fModuleName());
        QString lModuleVersion(lModuleInfo.fModuleVersion());
        if((lModuleName == APP_NAME) && (lCurrentModuleNamespace == APP_DOMAIN))
          continue;
        bool lStartModule = startmodule(lCurrentModuleNamespace, lModuleName, lModuleVersion, fRestartIfRunning);
        lAllModulesSuccessfullyStarted = lAllModulesSuccessfullyStarted && lStartModule;
      }
    }
    return lAllModulesSuccessfullyStarted;
  }

  bool NMainController::stopmodule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion) {
    QString lEffectiveModuleVersion = lModuleVersion.isEmpty() ? mModuleManager.fGetModuleLastVersion(lModuleNamespace, lModuleName) : lModuleVersion;
    if(lEffectiveModuleVersion.isEmpty()) {
      qDebug("Module '%s' does not exist!", lModuleName.toStdString().data());
      return false;
    }
    if((lModuleName == APP_NAME) && (lModuleNamespace == APP_DOMAIN)) {
      qDebug("Module '%s' can't be managed fron this interface!", lModuleName.toStdString().data());
      return false;
    }
    fStopModuleByScript(lModuleNamespace, lModuleName, lEffectiveModuleVersion);
    QString lProcessIndex(QString("%1%2%3%4%5").arg(lModuleNamespace).arg(cIndexSeparator).arg(lModuleName).arg(cIndexSeparator).arg(lEffectiveModuleVersion));
    if(mModulesRunning.contains(lProcessIndex)) {
      QProcess* rModuleProcess = mModulesRunning.value(lProcessIndex);
      if(rModuleProcess) {
        rModuleProcess->close();
        mModulesRunning.remove(lProcessIndex);
        rModuleProcess->deleteLater();
        qDebug("Closing module '%s'.", lModuleName.toStdString().data());
        return true;
      }
    }

    qDebug("Module '%s' with version '%s' is not running!", lModuleName.toStdString().data(), lEffectiveModuleVersion.toStdString().data());
    return false;
  }

  bool NMainController::stopallmodules(const QString &lModuleNamespace) {
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
        lAllModulesSuccessfullyStoped = lAllModulesSuccessfullyStoped && stopmodule(lCurrentModuleNamespace, lModuleName, lModuleVersion);
      }
    }
    return lAllModulesSuccessfullyStoped;
  }

  void NMainController::shutdownsystem(const TMessageRequestToProcess &lMessageRequest) {
    QVariantMap lShutdownSystemMap { {"shutdownsystem", QVariantMap() } };
    QString lResponseErrorCode;
    bool lShutdownSuccess = stopallmodules();
    NMessageResponse::EResponseStatus lMethodSuccess = NMessageResponse::EResponseStatus::eResponseSuccessful;
    if(!lShutdownSuccess) {
      lResponseErrorCode = QString("%1-%2").arg(fAbbreviation()).arg(int(lShutdownSuccess));
      lMethodSuccess = NMessageResponse::EResponseStatus::eResponseMethodExeError;
    }
    qint64 lResponseProcessingTime = NMessageResponse::fCalculateResponseProccessingTime(lMessageRequest.mMSecsSinceEpoch);
    NMessageResponse* lShutdownSystemResponse = new NMessageResponse(lMessageRequest.mWebSocketID, QString(), lMessageRequest.mMessageID, lResponseProcessingTime, lMethodSuccess, QString(), 0, lShutdownSystemMap, lResponseErrorCode);
    fSendMessage(lMessageRequest.mWebSocketsServerName, lShutdownSystemResponse);

    qApp->quit();
  }

  void NMainController::fFillMethodMetadata() {
    fAddMethodFunctionDescription(QStringLiteral("scanmanagedmodules"), tr("Searches and reads the configuration file Module.ncf in the directory hierarchy."));
    fAddMethodFunctionDescription(QStringLiteral("startallmodules"), tr("Starts modules of the desired namespace, if blank then all available modules are started."));
    fAddMethodFunctionDescription(QStringLiteral("startmodule"), tr("Starts the specified module."));
    fAddMethodFunctionDescription(QStringLiteral("stopmodule"), tr("Stops the specified module."));
    fAddMethodFunctionDescription(QStringLiteral("stopallmodules"), tr("Stops all modules belonging to the specified namespace, if blank it stops all modules that are currently running."));
    fAddMethodFunctionDescription(QStringLiteral("shutdownsystem"), tr("Stops the system enterily."));

    fAddMethodMinEventAndMinPeriod(QStringLiteral("scanmanagedmodules"), QString("0,0"));
    fAddMethodMinEventAndMinPeriod(QStringLiteral("startallmodules"), QString("0,0"));
    fAddMethodMinEventAndMinPeriod(QStringLiteral("startmodule"), QString("0,0"));
    fAddMethodMinEventAndMinPeriod(QStringLiteral("stopmodule"), QString("0,0"));
    fAddMethodMinEventAndMinPeriod(QStringLiteral("stopallmodules"), QString("0,0"));
    fAddMethodMinEventAndMinPeriod(QStringLiteral("shutdownsystem"), QString("0,0"));
  }

  void NMainController::fStopModuleByScript(const QString& lModuleNamespace, const QString& lModuleName, const QString& lEffectiveModuleVersion) {
      NModuleInfo lModuleInfo = mModuleManager.fModuleInfo(lModuleNamespace, lModuleName, lEffectiveModuleVersion);
      QString lStopScript = lModuleInfo.fModuleStopScript();
      if (lStopScript.isNull()) {
        qDebug("Module '%s' and version '%s' is not stoped by scirpt!", lModuleName.toStdString().data(), lEffectiveModuleVersion.toStdString().data());
        return;
      }
      QProcess* rModuleProcess = new QProcess(this);
      rModuleProcess->start(lStopScript);
      if(rModuleProcess->waitForStarted(cProcessStartMaxSeconds * 1000)) {
          QEventLoop lExtraDelay;
          QTimer lDelayTimer;
          connect(&lDelayTimer, &QTimer::timeout, &lExtraDelay, &QEventLoop::quit);
          lDelayTimer.start(cTimePeriod_ExtraDelayMSecs);
          lExtraDelay.exec();
      }      
      rModuleProcess->deleteLater();
  }
}
