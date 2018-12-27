#ifndef NMODULESMANAGER_H
#define NMODULESMANAGER_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QDir>
#include <QDebug>

#include "NModuleInfo.h"

namespace NulstarNS {
    class NModulesManager : public QObject {
        Q_OBJECT

    public:
        explicit NModulesManager(QObject *rParent = nullptr);
        ~NModulesManager() override { }

        NModuleInfo fModuleInfo(const QString lSpacename, const QString lModuleName, const QString lVersion);
        QStringList fGetNamespaces();
        QStringList fGetNamespaceModules(const QString lNamespace);
        QStringList fGetModuleVersions(const QString lModule);

    private:
        QMap<QString, QStringList > mNamespaceModules;    //NameSpace, Modules
        QMap<QString, QStringList > mModuleVersions;      //Module, Versions
        QMap<QPair<QString, QString>, QList<QPair<QString, QString>> > mModuleConfig;    //Module + Version, parameters pairs

        void fLoadModlesConfig();
        void fReadModuleNcf(const QString lNamespace, const QString lModule, const QString lVersion);
        void fScanModulesDirectory();
        QStringList fFoldersNameList(QString lDirPath);
        void fSetNamespaceModules(const QString lNamespace, QStringList lModules);
        void fSetModuleVersions(const QString lModule, QStringList lVersions);
        void fSetModuleConfig(const QString lModule, const QString lVersion, QList<QPair<QString, QString>> lParams);
        QList<QPair<QString, QString>>  fGetModuleConfig(const QString lModule, const QString lVersion);
    };
}

#endif // NMODULESMANAGER_H
