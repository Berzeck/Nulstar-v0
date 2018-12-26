#include <QCoreApplication>
#include <QSettings>

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
    const QString lModuleConfigGroupLibsAllowedNetworks("AllowedNetworks");
    const QString lModuleConfigGroupLibsCommPort("CommPort");

    const QString lModuleConfigGroupOutput("Output");
    const QString lModuleConfigGroupOutputLogLevel("LogLevel");

    const QString lModuleConfigGroupSecurity("Security");
    const QString lModuleConfigGroupSecuritySslMode("SslMode");


    NModulesManager::NModulesManager( QObject *rParent)
                 : QObject(rParent) {
        fLoadModlesConfig();
    }

    NModuleInfo NModulesManager::fModuleInfo(const QString lSpacename, const QString lModuleName, const QString lVersion){
        NModuleInfo lModuleInfo;
        QList<QPair<QString, QString>>  lModuleConfig = fGetModuleConfig(lModuleName, lVersion);
        if (0 == lModuleConfig.size()) {
            qDebug("Module(name %s, version %s) has not config params", lModuleName.toStdString().data(), lVersion.toStdString().data());
            return lModuleInfo;
        }

        QString lModuleAppName(QString("%1/../../../%2/%3/%4/%5").arg(QCoreApplication::applicationDirPath()).arg(lSpacename).arg(lModuleName).arg(lVersion).arg(lModuleName));
#ifdef Q_OS_WIN
        lModuleAppName.append(".exe");
#endif
        QStringList lModuleParametersList;
        QString lModuleLibDirPath(QString("%1/../../../../%2/").arg(QCoreApplication::applicationDirPath()).arg(lModuleConfigGroupLibs));
        QString lModuleWorkingDirectory(QString("%1/../../../%2/%3/%4/").arg(QCoreApplication::applicationDirPath()).arg(lSpacename).arg(lModuleName).arg(lVersion));

        QString lNustarVersion;
        QString lQtVersion;

        for (int i = 0; i < lModuleConfig.size(); ++i) {
            QPair<QString, QString> lParamPair = lModuleConfig.at(i);
            if (lParamPair.first == lModuleConfigGroupCoreLang.toLower()) {
                lModuleLibDirPath.append(lParamPair.second);
            }
            else if (lParamPair.first == lModuleConfigGroupLibsNulstar.toLower()) {
                lNustarVersion = lParamPair.second;
            }
            else if (lParamPair.first == lModuleConfigGroupLibsQt.toLower()) {
                lQtVersion = lParamPair.second;
            }
            else if ((lParamPair.first == lModuleConfigGroupLibsAllowedNetworks.toLower()) || (lParamPair.first == lModuleConfigGroupLibsCommPort.toLower()) ||
                     (lParamPair.first == lModuleConfigGroupOutputLogLevel.toLower()) || (lParamPair.first == lModuleConfigGroupSecuritySslMode.toLower()))
            {
                QString lFormattedParameter(lParamPair.first);
                lFormattedParameter.prepend("--");
                lModuleParametersList << lFormattedParameter << lParamPair.second;
            }
        }

        QString lModuleEnvLibsPath(QString("%1/%2/%3/").arg(lModuleLibDirPath).arg(lModuleConfigGroupLibsQt).arg(lQtVersion));
        lModuleEnvLibsPath.append(";");
        lModuleEnvLibsPath.append(QString("%1/%2/%3/").arg(lModuleLibDirPath).arg(lModuleConfigGroupLibsNulstar).arg(lNustarVersion));

        lModuleInfo.mModuleAppName = lModuleAppName;
        lModuleInfo.mModuleParametersList = lModuleParametersList;
        lModuleInfo.mModuleEnvLibsPath = lModuleEnvLibsPath;
        lModuleInfo.mModuleWorkingDirectory = lModuleWorkingDirectory;

        return lModuleInfo;
    }

    void NModulesManager::fLoadModlesConfig() {
        fScanModulesDirectory();

        for (const QString& lNamespace : mNamespaceModules.keys()){
            QStringList lModulesList = fGetNamespaceModules(lNamespace);
            QStringList::Iterator lModulesIterator;
            for (lModulesIterator = lModulesList.begin(); lModulesIterator != lModulesList.end(); ++lModulesIterator) {
                QStringList lVersionsList = fGetModuleVersions(*lModulesIterator);
                QStringList::Iterator lVersionsIterator;
                for (lVersionsIterator = lVersionsList.begin(); lVersionsIterator != lVersionsList.end(); ++lVersionsIterator) {
                    fReadModuleNcf(lNamespace, *lModulesIterator, *lVersionsIterator);
                }
            }
        }
    }

    void NModulesManager::fReadModuleNcf(QString lNamespace, QString lModule, QString lVersion){
        QString lModuleNcfPath =  QString("%1/../../../%2/%3/%4/%5").arg(QCoreApplication::applicationDirPath()).arg(lNamespace).arg(lModule).arg(lVersion).arg(lModuleConfigFile);
        QFileInfo lModuleNcfFileInfo(lModuleNcfPath);
        if (!lModuleNcfFileInfo.exists()){
            qDebug("Module(namespace %s, name %s, version %s) NCF file not exists",
                   lNamespace.toStdString().data(), lModule.toStdString().data(), lVersion.toStdString().data());
            return;
        }

        QList<QPair<QString, QString>> lParameters;
        bool lManaged = false;
        QSettings lModuleSettings(lModuleNcfPath, QSettings::IniFormat);
        QStringList lGroups(lModuleSettings.childGroups());
        for (const QString& lGroup : lGroups) {
            lModuleSettings.beginGroup(lGroup);
            QStringList lKeys(lModuleSettings.childKeys());
            for (const QString& lKey : lKeys) {
                QPair<QString, QString> lParameterPair;
                lParameterPair.first = lKey.toLower();
                lParameterPair.second = lModuleSettings.value(lKey).toString();
                lParameters << lParameterPair;
            }

            if (lGroup == lModuleConfigGroupCore) {
                lManaged = lModuleSettings.value(lModuleConfigGroupCoreManaged, false).toBool();
            }
            lModuleSettings.endGroup();
        }

        if (lManaged){
            fSetModuleConfig(lModule, lVersion, lParameters);
        }
    }

    void NModulesManager::fScanModulesDirectory() {
       QString lModulesDirPath =  QString("%1/../../../").arg(QCoreApplication::applicationDirPath());
       QStringList lNameSpaceList = fFoldersNameList(lModulesDirPath);
       if (lNameSpaceList.isEmpty()){
           qDebug() << "Namespace directory not exist under Modules directory " << lModulesDirPath;
           return;
       }

       QStringList::Iterator lNSIterator;
       QString lNSModulesDirPath = QString();
       QStringList lNSModulesList = QStringList();
       for (lNSIterator = lNameSpaceList.begin(); lNSIterator != lNameSpaceList.end(); ++lNSIterator) {
            QString lModulesDirPathTemp = lModulesDirPath;
            lNSModulesDirPath = lModulesDirPathTemp.append("%1/").arg(*lNSIterator);
            lNSModulesList = fFoldersNameList(lNSModulesDirPath);
            if (lNSModulesList.isEmpty()){
                qDebug() << "Module directory not exist under Namespace directory " << lNSModulesDirPath;
                continue;
            }
            fSetNamespaceModules(*lNSIterator, lNSModulesList);

            QStringList::Iterator lModuleIterator;
            QString lModuleVersionsDirPath = QString();
            QStringList lModuleVersionsList = QStringList();
            for (lModuleIterator = lNSModulesList.begin(); lModuleIterator != lNSModulesList.end(); ++lModuleIterator) {
                 QString lNSModulesDirPathTemp = lNSModulesDirPath;
                 lModuleVersionsDirPath = lNSModulesDirPathTemp.append("%1/").arg(*lModuleIterator);
                 lModuleVersionsList = fFoldersNameList(lModuleVersionsDirPath);
                 if (lModuleVersionsList.isEmpty()){
                     qDebug() << "Versions directory not exist under Module directory " << lModuleVersionsDirPath;
                     continue;
                 }
                 fSetModuleVersions(*lModuleIterator, lModuleVersionsList);
            }
        }
    }

    QStringList NModulesManager::fFoldersNameList(QString lDirPath){
        QStringList lFolderNameList = QStringList();
        QDir lDir(lDirPath);
        if(!lDir.exists()) {
            qDebug("Modules directory %s not exist", lDirPath.toStdString().data());
            return lFolderNameList;
        }
        QFileInfoList lFolderFilesList = lDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(int i = 0; i != lFolderFilesList.size(); i++) {
            lFolderNameList.append(lFolderFilesList.at(i).fileName());
        }
        return lFolderNameList;
    }

    void NModulesManager::fSetNamespaceModules(const QString lNamespace, QStringList lModules) {
      mNamespaceModules[lNamespace] = lModules;
    }

    void NModulesManager::fSetModuleVersions(const QString lModule, QStringList lVersions) {
      mModuleVersions[lModule] = lVersions;
    }

    void NModulesManager::fSetModuleConfig(const QString lModule, const QString lVersion, QList<QPair<QString, QString>> lParams){
        QPair<QString, QString> lModuleVersion;
        lModuleVersion.first = lModule.toLower();
        lModuleVersion.second = lVersion.toLower();
        mModuleConfig[lModuleVersion] = lParams;
    }

    QStringList NModulesManager::fGetNamespaceModules(const QString lNamespace) {
      return mNamespaceModules[lNamespace];
    }

    QStringList NModulesManager::fGetModuleVersions(const QString lModule) {
      return mModuleVersions[lModule];
    }

     QList<QPair<QString, QString>>  NModulesManager::fGetModuleConfig(const QString lModule, const QString lVersion){
        QPair<QString, QString> lModuleVersion;
        lModuleVersion.first = lModule.toLower();
        lModuleVersion.second = lVersion.toLower();
        return mModuleConfig[lModuleVersion];
    }

 }
