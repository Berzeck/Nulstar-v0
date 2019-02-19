#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QVersionNumber>
#include "NModuleManager.h"


namespace NulstarNS {
  const QString cModuleConfigFile("Module.ncf");
  const QString cModuleConfigGroupCore("Core");
  const QString cModuleConfigGroupCoreLang("Language");
  const QString cModuleConfigGroupCoreManaged("Managed");
  const QString cModuleConfigGroupLibs("Libraries");
  const QString cModuleConfigGroupLibsJre("JRE");
  const QString cModuleConfigGroupScript("Script");

  NModuleManager::NModuleManager(QObject *rParent)
                 : QObject(rParent), mModulesConfigLoaded(false) {

  }

  NModuleInfo NModuleManager::fModuleInfo(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion) {
    if(!mModulesConfigLoaded)
      fLoadModulesInfo();

    QString lCurrentModuleVersion(lModuleVersion);
    if(lCurrentModuleVersion.isEmpty())
      lCurrentModuleVersion = fGetModuleLastVersion(lModuleNamespace, lModuleName);

    for(const NModuleInfo& lModuleInfo : mModuleInfoList ) {
      if((lModuleInfo.fModuleNamespace() == lModuleNamespace) && (lModuleInfo.fModuleName() == lModuleName) && (lModuleInfo.fModuleVersion() == lCurrentModuleVersion))
        return lModuleInfo;
    }
    return NModuleInfo(QString(), QString(), QString(), QString(), QString(), QStringList(), QList<NModuleParameter>(), QList<NModuleParameter>());
  }

  quint8 NModuleManager::fLoadModulesInfo() {
    quint8 lTotalModules = 0;
    mNamespaceModules.clear();
    mModuleVersions.clear();
    mModuleInfoList.clear();

    fScanManagedModules();
    mModulesConfigLoaded = true;
    for(const QString& lModuleNamespace : mNamespaceModules.keys()){
      QStringList lModuleList = fGetNamespaceModules(lModuleNamespace);
      for(const QString& lModuleName : lModuleList) {
        QStringList lVersionList = fGetModuleVersions(lModuleNamespace, lModuleName);
        for(const QString& lModuleVersion : lVersionList) {
          bool lReadModuleNcfSuccess = fReadModuleNcf(lModuleNamespace, lModuleName, lModuleVersion);
          if(lReadModuleNcfSuccess)
            ++lTotalModules;
        }
      }
    }
    return lTotalModules;
  }

  QList<NModuleInfo> NModuleManager::fModuleInfoList() {
    if(!mModulesConfigLoaded)
      fLoadModulesInfo();
    return mModuleInfoList;
  }

  bool NModuleManager::fReadModuleNcf(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion){
    QString lModuleNcfPath = QString("%1/../../../%2/%3/%4/%5").arg(QCoreApplication::applicationDirPath()).arg(lModuleNamespace).arg(lModuleName).arg(lModuleVersion).arg(cModuleConfigFile);
    QFileInfo lModuleNcfFileInfo(lModuleNcfPath);
    if(!lModuleNcfFileInfo.exists()){
      qDebug("Module(namespace %s, name %s, version %s) NCF file not exists", lModuleNamespace.toStdString().data(), lModuleName.toStdString().data(), lModuleVersion.toStdString().data());
      return false;
    }

    QString lModulesDirNcfPath = QString("%1/../../../%2").arg(QCoreApplication::applicationDirPath()).arg(cModuleConfigFile);
    QString lModulesNsNcfPath = QString("%1/../../../%2/%3").arg(QCoreApplication::applicationDirPath()).arg(lModuleNamespace).arg(cModuleConfigFile);
    QList<NModuleParameter> lParameters;

    fReadModuleNcf(lModulesDirNcfPath, lParameters)->fReadModuleNcf(lModulesNsNcfPath, lParameters)->fReadModuleNcf(lModuleNcfPath, lParameters);

    bool lManaged = bool(fGetModuleParamsValue(cModuleConfigGroupCore, cModuleConfigGroupCoreManaged, lParameters) == QString("%1").arg(1));
    if(lManaged){
      fSetModuleInfo(lModuleNamespace, lModuleName, lModuleVersion, lParameters);
      return true;
    }
    return false;
  }

  NModuleManager* NModuleManager::fReadModuleNcf(const QString& lNcfPath, QList<NModuleParameter>& lParameters){
    QFileInfo lModuleNcfFileInfo(lNcfPath);
    if(!lModuleNcfFileInfo.exists()){
      return this;
    }

    QSettings lModuleSettings(lNcfPath, QSettings::IniFormat);
    QStringList lGroups(lModuleSettings.childGroups());
    for(const QString& lGroup : lGroups) {
      lModuleSettings.beginGroup(lGroup);
      QStringList lKeys(lModuleSettings.childKeys());
      for (const QString& lKey : lKeys) {
         NModuleParameter lParameterStructure;
         lParameterStructure.mGroupName = lGroup;
         lParameterStructure.mParamName = lKey;
         lParameterStructure.mParamValue = lModuleSettings.value(lKey).toString();

         QString lValue = fGetModuleParamsValue(lParameterStructure.mGroupName, lParameterStructure.mParamName, lParameters);
         if (!lValue.isEmpty()) {
            NModuleParameter lParameterStructureRm;
            lParameterStructureRm.mGroupName = lGroup;
            lParameterStructureRm.mParamName = lKey;
            lParameterStructureRm.mParamValue = lValue;
            lParameters.removeOne(lParameterStructureRm);
         }
         lParameters << lParameterStructure;
      }
      lModuleSettings.endGroup();
    }

    return this;
  }

  QString NModuleManager::fGetModuleParamsValue(const QString& lModuleParamGroup, const QString& lModuleParamKey, const QList<NModuleParameter>& lModuleParameters)
  {
      for(const NModuleParameter& lModuleParameter : lModuleParameters ) {
          if((lModuleParameter.mGroupName == lModuleParamGroup) && (lModuleParameter.mParamName == lModuleParamKey)) {
              return lModuleParameter.mParamValue;
          }
      }
      return QString();
  }

  void NModuleManager::fScanManagedModules() {
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
        fSetModuleVersions(lNameSpace, lModuleVersion, lModuleVersionsList);
      }
    }
  }

  QStringList NModuleManager::fFoldersNameList(const QString& lDirPath) const {
    QDir lDir(lDirPath);
    if(!lDir.exists()) {
      qDebug("Modules directory %s not exist", lDirPath.toStdString().data());
      return QStringList();
    }
    QStringList lFolderNameList = QStringList(lDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    return lFolderNameList;
  }

  void NModuleManager::fSetNamespaceModules(const QString &lModuleNamespace, const QStringList &lModuleNames) {
    mNamespaceModules[lModuleNamespace] = lModuleNames;
  }

  void NModuleManager::fSetModuleVersions(const QString& lModuleNamespace, const QString& lModuleName, const QStringList& lModuleVersions) {
    QString lModuleIndex(QString("%1%2%3").arg(lModuleNamespace).arg(cIndexSeparator).arg(lModuleName));
    mModuleVersions[lModuleIndex] = lModuleVersions;
  }

  void NModuleManager::fSetModuleInfo(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion, const QList<NModuleParameter>& lModuleParameters) {
    QString lModuleLanguage = fGetModuleParamsValue(cModuleConfigGroupCore, cModuleConfigGroupCoreLang, lModuleParameters);
    QString lModuleWorkingDirectory(QString("%1/../../../%2/%3/%4/").arg(QCoreApplication::applicationDirPath()).arg(lModuleNamespace).arg(lModuleName).arg(lModuleVersion));
    lModuleWorkingDirectory = QDir(lModuleWorkingDirectory).canonicalPath();

    QString lModuleLibDirPath(QString("%1/../../../../%2").arg(QCoreApplication::applicationDirPath()).arg(cModuleConfigGroupLibs));
    QStringList lLibDirPaths;
    QList<NModuleParameter> lModuleStartupParameters;
    QList<NModuleParameter> lModuleScriptParams;
    for(const NModuleParameter& lModuleParameter : lModuleParameters ) {
      if(lModuleParameter.mGroupName == cModuleConfigGroupLibs) {
        QDir lLibDir(QString("%1/%2/%3/%4/").arg(lModuleLibDirPath).arg(lModuleLanguage).arg(lModuleParameter.mParamName).arg(lModuleParameter.mParamValue));
        if(lLibDir.exists()){
          lLibDirPaths.append(lLibDir.canonicalPath());
          if (lModuleParameter.mParamName == cModuleConfigGroupLibsJre){
              NModuleParameter lModuleParam;
              lModuleParam.mGroupName = lModuleParameter.mGroupName;
              lModuleParam.mParamName = lModuleParameter.mParamName;
              lModuleParam.mParamValue = lLibDir.canonicalPath();
              lModuleStartupParameters << lModuleParam;
          }
        }
      }
      else if(lModuleParameter.mGroupName == cModuleConfigGroupScript) {
         lModuleScriptParams << lModuleParameter;
      }
      else if(lModuleParameter.mGroupName != cModuleConfigGroupCore){
         lModuleStartupParameters << lModuleParameter;
      }
    }

    NModuleInfo lModuleInfo(lModuleName, lModuleNamespace, lModuleVersion, lModuleLanguage, lModuleWorkingDirectory, lLibDirPaths, lModuleStartupParameters, lModuleScriptParams);
    mModuleInfoList << lModuleInfo;
  }

  QStringList NModuleManager::fGetNamespaceModules(const QString& lNameSpace) {
    if(!mModulesConfigLoaded)
      fLoadModulesInfo();
    QStringList lNameSpaceModules;
    if(mNamespaceModules.contains(lNameSpace))
      lNameSpaceModules = mNamespaceModules.value(lNameSpace);
    return lNameSpaceModules;
  }

  QStringList NModuleManager::fGetModules() {
    if(!mModulesConfigLoaded)
      fLoadModulesInfo();
    return mModuleVersions.keys();
  }

  QStringList NModuleManager::fGetModuleVersions(const QString &lModuleNamespace, const QString& lModuleName) {
    if(!mModulesConfigLoaded)
      fLoadModulesInfo();
    QStringList lModuleVersions;
    QString lMapIndex(QString("%1%2%3").arg(lModuleNamespace).arg(cIndexSeparator).arg(lModuleName));
    if(mModuleVersions.contains(lMapIndex))
      lModuleVersions = mModuleVersions.value(lMapIndex);
    return lModuleVersions;
  }

  QString NModuleManager::fGetModuleLastVersion(const QString& lModuleNamespace, const QString& lModuleName)  {
    if(!mModulesConfigLoaded)
      fLoadModulesInfo();
    QStringList lModuleVersions = fGetModuleVersions(lModuleNamespace, lModuleName);
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

  QStringList NModuleManager::fGetNamespaces() {
    if(!mModulesConfigLoaded)
      fLoadModulesInfo();
    QStringList lNamespacesList(mNamespaceModules.keys());
    return lNamespacesList;
  }
}
