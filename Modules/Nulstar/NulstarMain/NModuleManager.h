#ifndef NMODULESMANAGER_H
#define NMODULESMANAGER_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QDebug>

#include "NModuleInfo.h"

namespace NulstarNS {  
  const QString cIndexSeparator("--");

  class NModuleManager : public QObject {
    Q_OBJECT

  public:
    explicit NModuleManager(QObject *rParent = nullptr);
    ~NModuleManager() override { }

    NModuleInfo fModuleInfo(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion = QString());
    QList<NModuleInfo> fModuleInfoList();
    QStringList fGetNamespaces();
    QStringList fGetNamespaceModules(const QString& lNameSpace);
    QStringList fGetModules();
    QStringList fGetModuleVersions(const QString& lModuleNamespace, const QString& lModuleName);
    QString fGetModuleLastVersion(const QString& lModuleNamespace, const QString& lModuleName);
    quint8 fLoadModulesInfo();

  private:
    bool mModulesConfigLoaded;
    QMap<QString, QStringList > mNamespaceModules;    //NameSpace, Modules
    QMap<QString, QStringList > mModuleVersions;      //Namespace--Module, Versions
    QList<NModuleInfo> mModuleInfoList;

    bool fReadModuleNcf(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion);
    void fScanManagedModules();
    QStringList fFoldersNameList(const QString& lDirPath) const;
    void fSetNamespaceModules(const QString& lModuleNamespace, const QStringList& lModuleNames);
    void fSetModuleVersions(const QString& lModuleNamespace, const QString &lModuleName, const QStringList& lModuleVersions);
    void fSetModuleInfo(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion, const QList<SModuleParameter>& lModuleParameters);
    NModuleManager* fReadModuleNcf(const QString& lNcfPath, QList<SModuleParameter>& lParameters);
    QString fGetModuleParamsValue(const QString& lModuleParamGroup, const QString& lModuleParamKey, const QList<SModuleParameter>& lModuleParameters);
  };
}

#endif // NMODULESMANAGER_H
