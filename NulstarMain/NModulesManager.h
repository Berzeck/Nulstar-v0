#ifndef NMODULESMANAGER_H
#define NMODULESMANAGER_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QDebug>

#include "NModuleInfo.h"

namespace NulstarNS {
  struct SModuleConfigParams {
    QString mGroupName;
    QString mParamName;
    QString mParamValue;
  };

  class NModulesManager : public QObject {
    Q_OBJECT

  public:
    explicit NModulesManager(QObject *rParent = nullptr);
    ~NModulesManager() override { }

    NModuleInfo fModuleInfo(const QString& lSpaceName, const QString& lModuleName, const QString& lVersion);
    QStringList fGetNamespaces();
    QStringList fGetNamespaceModules(const QString& lNameSpace);
    QStringList fGetModuleVersions(const QString& lModule);
    QString fGetModuleLastVersion(const QString& lModule);
    void fLoadModulesConfig();

  private:
    bool mModulesConfigLoaded;
    QMap<QString, QStringList > mNamespaceModules;    //NameSpace, Modules
    QMap<QString, QStringList > mModuleVersions;      //Module, Versions
    QMap<QString, QList<SModuleConfigParams>> mModuleConfig;    //Module--Version, parameters strucutres
    QStringList mModuleExeOptionList;

    void fReadModuleNcf(const QString& lNamespace, const QString& lModule, const QString& lVersion);
    void fScanModulesDirectory();
    QStringList fFoldersNameList(const QString& lDirPath) const;
    void fSetNamespaceModules(const QString& lNamespace, const QStringList& lModules);
    void fSetModuleVersions(const QString& lModule, const QStringList& lVersions);
    void fSetModuleConfig(const QString& lModule, const QString& lVersion, const QList<SModuleConfigParams>& lParams);
    QList<SModuleConfigParams> fGetModuleConfig(const QString& lModule, const QString& lVersion) const;
  };
}

#endif // NMODULESMANAGER_H
