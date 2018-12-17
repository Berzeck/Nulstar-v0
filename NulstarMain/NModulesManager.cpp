#include <QCoreApplication>


#include "NModulesManager.h"


namespace NulstarNS {
    const QString lModulesDirectory("Modules");
    const QString lModulesNSNulstar("Nulstar");
    const QString lModulesNSNuls("Nuls");

    const QString lModulesNS("ModuleSpaceName");
    const QString lModulesVersion("ModuleVersion");

    const QString lModuleConfigFile("Module.ncf");

    const QString lModuleConfigGroupCore("Core");
    const QString lModuleConfigGroupCoreLang("Core-Language");
    const QString lModuleConfigGroupCoreManaged("Core-Managed");

    const QString lModuleConfigGroupLibs("Libraries");
    const QString lModuleConfigGroupLibsNulstar("Libraries-Nulstar");
    const QString lModuleConfigGroupLibsQt("Libraries-Qt");

    const QString lModuleConfigGroupNetwork("Network");
    const QString lModuleConfigGroupLibsAllowedNetworks("Network-AllowedNetworks");
    const QString lModuleConfigGroupLibsCommPort("Network-CommPort");

    const QString lModuleConfigGroupOutput("Output");
    const QString lModuleConfigGroupOutputLogLevel("Output-LogLevel");

    const QString lModuleConfigGroupSecurity("Security");
    const QString lModuleConfigGroupSecuritySslMode("Security-SslMode");


    NModulesManager::NModulesManager( QObject *rParent)
                 : QObject(rParent) {
       fMoudlesReadConfig();
    }

    void NModulesManager::fMoudlesReadConfig() {
       QString lModulesDirPath =  QString("%1/%2/").arg(QCoreApplication::applicationDirPath()).arg(lModulesDirectory);
       QDir lModulesDir(lModulesDirPath);
       if(!lModulesDir.exists())
       {
           qDebug() << "Modules directory %s is not exist" << lModulesDirPath;
           return;
       }
       //get namespaces
       QStringList lNameSpaceList = fFoldersNameList(lModulesDir);
       if (lNameSpaceList.isEmpty()){
           qDebug() << "Namespace directory is not exist under Module directory %s" << lModulesDirPath;
           return;
       }




    }

    QStringList NModulesManager::fFoldersNameList(QDir lDir){
        QStringList lFolderNameList = QStringList();
        QFileInfoList lFolderFilesList = lDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(int i = 0; i != lFolderFilesList.size(); i++)
        {
             lFolderNameList.append(lFolderFilesList.at(i).fileName());
        }
        return lFolderNameList;
    }

    void NModulesManager::fSetNamespaceModules(QString lNamespace, QStringList lModules) {
      mNamespaceModules[lNamespace] = lModules;
    }

    void NModulesManager::fSetModuleVersions(QString lModule, QStringList lVersions) {
      mModuleVersions[lModule] = lVersions;
    }

    void NModulesManager::fSetModuleConfig(QString lModule, QString lVersion, QList<QPair<QString, QString>> lParams){
        QPair<QString, QString> lModuleVersion;
        lModuleVersion.first = lModule.toLower();
        lModuleVersion.second = lVersion.toLower();
        mModuleConfig[lModuleVersion] = lParams;
    }
 }
