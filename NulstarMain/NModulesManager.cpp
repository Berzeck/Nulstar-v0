#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QVersionNumber>
#include "NModulesManager.h"


namespace NulstarNS {
    const QString lModulesDirectory("Modules");
    const QString lModuleConfigFile("Module.ncf");

    const QString lModuleConfigGroupCore("Core");
    const QString lModuleConfigGroupCoreLang("Language");
    const QString lModuleConfigGroupCoreManaged("Managed");

    const QString lModuleConfigGroupLibs("Libraries");
    const QString lModuleConfigGroupLibsNulstar("Nulstar");
    const QString lModuleConfigGroupLibsQt("Qt");

    const QString lModuleConfigGroupNetwork("Network");
    const QString lModuleConfigGroupNwAllowedNetworks("AllowedNetworks");
    const QString lModuleConfigGroupNwCommPort("CommPort");
    const QString lModuleConfigGroupNwMainControllerIP("MainControllerIP");
    const QString lModuleConfigGroupNwAdminPort("AdminPort");
    const QString lModuleConfigGroupNwClientPort("ClientPort");

    const QString lModuleConfigGroupOutput("Output");
    const QString lModuleConfigGroupOutputLogLevel("LogLevel");

    const QString lModuleConfigGroupSecurity("Security");
    const QString lModuleConfigGroupSecuritySslMode("SslMode");


    NModulesManager::NModulesManager( QObject *rParent)
                   : QObject(rParent), mModulesConfigLoaded(false) {
      mModuleExeOptionList.append(lModuleConfigGroupNwAllowedNetworks.toLower());
      mModuleExeOptionList.append(lModuleConfigGroupNwCommPort.toLower());
      mModuleExeOptionList.append(lModuleConfigGroupNwMainControllerIP.toLower());
      mModuleExeOptionList.append(lModuleConfigGroupNwAdminPort.toLower());
      mModuleExeOptionList.append(lModuleConfigGroupNwClientPort.toLower());
      mModuleExeOptionList.append(lModuleConfigGroupOutputLogLevel.toLower());
      mModuleExeOptionList.append(lModuleConfigGroupSecuritySslMode.toLower());
    }

    NModuleInfo NModulesManager::fModuleInfo(const QString& lSpaceName, const QString& lModuleName, const QString& lVersion) {
      if(!mModulesConfigLoaded)
        fLoadModulesConfig();

      NModuleInfo lModuleInfo(lModuleName);
      QList<SModuleConfigParams> lModuleConfig = fGetModuleConfig(lModuleName, lVersion);
      if(lModuleConfig.size() == 0) {
         qDebug("Module(name %s, version %s) has not config params", lModuleName.toStdString().data(), lVersion.toStdString().data());
         return lModuleInfo;
      }

      QString lModuleAppName(QString("%1/../../../%2/%3/%4/%5").arg(QCoreApplication::applicationDirPath()).arg(lSpaceName).arg(lModuleName).arg(lVersion).arg(lModuleName));
#ifdef Q_OS_WIN
      lModuleAppName.append(".exe");
#endif
      QDir lModuleAppNameDir(lModuleAppName);

      QMap<QString, QString> lModuleParameters;
      QString lModuleLibDirPath(QString("%1/../../../../%2/").arg(QCoreApplication::applicationDirPath()).arg(lModuleConfigGroupLibs));
      QString lModuleWorkingDirectory(QString("%1/../../../%2/%3/%4/").arg(QCoreApplication::applicationDirPath()).arg(lSpaceName).arg(lModuleName).arg(lVersion));
      lModuleWorkingDirectory = QDir(lModuleWorkingDirectory).canonicalPath();

      QString lModuleLanguage;
      QStringList lLibDirPaths;

      for (int i = 0; i < lModuleConfig.size(); ++i) { // Must ALWAYS find first!
        SModuleConfigParams lParamStructure = lModuleConfig.at(i);
        if((lParamStructure.mGroupName == lModuleConfigGroupCore) && (lParamStructure.mParamName == lModuleConfigGroupCoreLang)) {
          lModuleLanguage = lParamStructure.mParamValue;
        }
      }
      for (int i = 0; i < lModuleConfig.size(); ++i) {
        SModuleConfigParams lParamStructure = lModuleConfig.at(i);
        if(lParamStructure.mGroupName == lModuleConfigGroupLibs) {
          QDir lLibDir(QString("%1/%2/%3/%4/").arg(lModuleLibDirPath).arg(lModuleLanguage).arg(lParamStructure.mParamName).arg(lParamStructure.mParamValue));
          if(lLibDir.exists())
            lLibDirPaths.append(lLibDir.canonicalPath());
        }
        else if(mModuleExeOptionList.contains(lParamStructure.mParamName.toLower())) {
           lModuleParameters[lParamStructure.mParamName] = lParamStructure.mParamValue;
        }
      }
      lModuleInfo.fSetModuleLanguage(lModuleLanguage);
      lModuleInfo.fSetModuleParameters(lModuleParameters);
      lModuleInfo.fSetModuleEnvLibPaths(lLibDirPaths);
      lModuleInfo.fSetModuleWorkingDirectory(lModuleWorkingDirectory);
      return lModuleInfo;
  }

  void NModulesManager::fLoadModulesConfig() {
    mNamespaceModules.clear();
    mModuleVersions.clear();
    mModuleConfig.clear();

    fScanModulesDirectory();
    mModulesConfigLoaded = true;
    for(const QString& lNamespace : mNamespaceModules.keys()){
      QStringList lModuleList = fGetNamespaceModules(lNamespace);
      //for(lModulesIterator = lModulesList.begin(); lModulesIterator != lModulesList.end(); ++lModulesIterator) {
      for(const QString& lModuleName : lModuleList) {
        QStringList lVersionList = fGetModuleVersions(lModuleName);
        for(const QString& lVersion : lVersionList) {
          fReadModuleNcf(lNamespace, lModuleName, lVersion);
        }
      }
    }
  }

  void NModulesManager::fReadModuleNcf(const QString& lNamespace, const QString& lModule, const QString& lVersion){
    QString lModuleNcfPath = QString("%1/../../../%2/%3/%4/%5").arg(QCoreApplication::applicationDirPath()).arg(lNamespace).arg(lModule).arg(lVersion).arg(lModuleConfigFile);
    QFileInfo lModuleNcfFileInfo(lModuleNcfPath);
    if(!lModuleNcfFileInfo.exists()){
      qDebug("Module(namespace %s, name %s, version %s) NCF file not exists", lNamespace.toStdString().data(), lModule.toStdString().data(), lVersion.toStdString().data());
      return;
    }

    QList<SModuleConfigParams> lParameters;
    bool lManaged = false;
    QSettings lModuleSettings(lModuleNcfPath, QSettings::IniFormat);
    QStringList lGroups(lModuleSettings.childGroups());
    for(const QString& lGroup : lGroups) {
      lModuleSettings.beginGroup(lGroup);
      QStringList lKeys(lModuleSettings.childKeys());
      for (const QString& lKey : lKeys) {
         SModuleConfigParams lParameterStructure;
         lParameterStructure.mGroupName = lGroup;
         lParameterStructure.mParamName = lKey;
         lParameterStructure.mParamValue = lModuleSettings.value(lKey).toString();
         lParameters << lParameterStructure;
      }
      if(lGroup == lModuleConfigGroupCore) {
        lManaged = lModuleSettings.value(lModuleConfigGroupCoreManaged, false).toBool();
      }
      lModuleSettings.endGroup();
    }
    if(lManaged){
      fSetModuleConfig(lModule, lVersion, lParameters);
    }
  }

  void NModulesManager::fScanModulesDirectory() {
    QString lModulesDirPath =  QString("%1/../../../").arg(QCoreApplication::applicationDirPath());
    QStringList lNameSpaceList = fFoldersNameList(lModulesDirPath);
    if(lNameSpaceList.isEmpty()){
      qDebug() << "Namespace directory not exist under Modules directory " << lModulesDirPath;
      return;
    }
    for(const QString& lNameSpace : lNameSpaceList) {
      QString lModulesDirPathTemp = lModulesDirPath;
      QString lNSModulesDirPath = lModulesDirPathTemp.append("%1/").arg(lNameSpace);
      QStringList lNSModulesList = fFoldersNameList(lNSModulesDirPath);
      if(lNSModulesList.isEmpty()){
        qDebug() << "Module directory not exist under Namespace directory " << lNSModulesDirPath;
        continue;
      }
      fSetNamespaceModules(lNameSpace, lNSModulesList);

      QStringList lModuleVersionsList = QStringList();
      for(const QString& lModuleVersion : lNSModulesList) {
        QString lNSModulesDirPathTemp = lNSModulesDirPath;
        QString lModuleVersionsDirPath = lNSModulesDirPathTemp.append("%1/").arg(lModuleVersion);
        lModuleVersionsList = fFoldersNameList(lModuleVersionsDirPath);
        if(lModuleVersionsList.isEmpty()){
          qDebug() << "Versions directory not exist under Module directory " << lModuleVersionsDirPath;
          continue;
        }
        fSetModuleVersions(lModuleVersion, lModuleVersionsList);
      }
    }
  }

  QStringList NModulesManager::fFoldersNameList(const QString& lDirPath) const {    
    QDir lDir(lDirPath);
    if(!lDir.exists()) {
      qDebug("Modules directory %s not exist", lDirPath.toStdString().data());
      return QStringList();
    }
    QStringList lFolderNameList = QStringList(lDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    return lFolderNameList;
  }

  void NModulesManager::fSetNamespaceModules(const QString &lNamespace, const QStringList &lModules) {
    mNamespaceModules[lNamespace] = lModules;
  }

  void NModulesManager::fSetModuleVersions(const QString& lModule, const QStringList& lVersions) {
    mModuleVersions[lModule] = lVersions;
  }

  void NModulesManager::fSetModuleConfig(const QString& lModule, const QString& lVersion, const QList<SModuleConfigParams>& lParams) {
    QString lModuleVersion = QString("%1--%2").arg(lModule).arg(lVersion);
    mModuleConfig[lModuleVersion] = lParams;
  }

  QStringList NModulesManager::fGetNamespaceModules(const QString& lNameSpace) {
    if(!mModulesConfigLoaded)
      fLoadModulesConfig();
    QStringList lNameSpaceModules;
    if(mNamespaceModules.contains(lNameSpace))
      lNameSpaceModules = mNamespaceModules.value(lNameSpace);
    return lNameSpaceModules;
  }

  QStringList NModulesManager::fGetModuleVersions(const QString& lModule) {
    if(!mModulesConfigLoaded)
      fLoadModulesConfig();
    QStringList lModuleVersions;
    if(mModuleVersions.contains(lModule))
      lModuleVersions = mModuleVersions.value(lModule);
    return lModuleVersions;
  }

  QString NModulesManager::fGetModuleLastVersion(const QString& lModule)  {
    if(!mModulesConfigLoaded)
      fLoadModulesConfig();
    QStringList lModuleVersions = fGetModuleVersions(lModule);
    if(lModuleVersions.size() == 0)
      return QString();
    QVersionNumber lLastVersion;
    for(const QString& lVersion : lModuleVersions) {
      QVersionNumber lCurrentVersion = QVersionNumber::fromString(lVersion);
      if(lLastVersion.isNull() || (lCurrentVersion > lLastVersion ))
        lLastVersion = lCurrentVersion;
    }
    if(lLastVersion.isNull())
      return QString();
    return lLastVersion.toString();
  }

  QList<SModuleConfigParams>  NModulesManager::fGetModuleConfig(const QString &lModule, const QString &lVersion) const{
    QList<SModuleConfigParams> lConfigList;
    QString lModuleVersion = QString("%1--%2").arg(lModule).arg(lVersion);
    if(mModuleConfig.contains(lModuleVersion))
      lConfigList = mModuleConfig.value(lModuleVersion);
    return lConfigList;
  }

  QStringList NModulesManager::fGetNamespaces() {
    if(!mModulesConfigLoaded)
      fLoadModulesConfig();
    QStringList lNamespacesList(mNamespaceModules.keys());
    return lNamespacesList;
  }
}
