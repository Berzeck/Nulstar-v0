#ifndef NMODULESMANAGER_H
#define NMODULESMANAGER_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QDir>
#include <QDebug>

namespace NulstarNS {
  class NModulesManager : public QObject {
    Q_OBJECT
    class NModuleInfo;
    public:      
      explicit NModulesManager(QObject *rParent = nullptr);
      ~NModulesManager() override { }


     NModuleInfo fModuleInfo(QString lModuleName, QString lVersion);


    private:
        QMap<QString, QStringList > mNamespaceModules;    //NameSpace, Modules
        QMap<QString, QStringList > mModuleVersions;      //Module, Versions
        QMap<QPair<QString, QString>, QList<QPair<QString, QString>> > mModuleConfig;    //Module + Version, parameters pairs

        void fMoudlesReadConfig();
        QStringList fFoldersNameList(QDir lDir);
        void fSetNamespaceModules(QString lNamespace, QStringList lModules);
        void fSetModuleVersions(QString lModule, QStringList lVersions);
        void fSetModuleConfig(QString lModule, QString lVersion, QList<QPair<QString, QString>> lParams);

  };
}

#endif // NMODULESMANAGER_H
