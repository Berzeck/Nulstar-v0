#ifndef NMAINCONTROLLER_H
#define NMAINCONTROLLER_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QVersionNumber>

#include <NCoreService.h>
#include "NulstarVersion.h"
#include "NModuleManager.h"

class QProcess;

namespace NulstarNS {
  const QString cServiceManager("ServiceManager");

  class NMainController : public NCoreService {
    Q_OBJECT

    public:
      explicit NMainController();
      ~NMainController() override {}

      QString fAbbreviation() const override { return "NMC"; }
      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QVariantMap fDependencies() const override {  QVariantMap lDependencies( {{cRole_ServiceManager, cVersion_ServiceManagerRole}} ); return lDependencies; }
      QVariantMap fApiRoles() const override;
      QList<QVersionNumber> fProtocolVersionsSupported() const override { QList<QVersionNumber> lApiVersionsSupported; QVersionNumber lMainVersion(QVersionNumber::fromString(APP_PROTOCOL_VERSIONS)); lApiVersionsSupported << lMainVersion; return lApiVersionsSupported; }

    public Q_SLOTS:
      API_ADMIN_FUNCTION quint8 fScanManagedModules();
      API_ADMIN_FUNCTION bool fStartModule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion = QString(), bool fRestartIfRunning = false);
      API_ADMIN_FUNCTION bool fStartAllModules(const QString& lModuleNamespace = QString(), bool fRestartIfRunning = false);
      API_ADMIN_FUNCTION bool fStopModule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion = QString());
      API_ADMIN_FUNCTION bool fStopAllModules(const QString& lModuleNamespace = QString());
      API_ADMIN_FUNCTION void fShutdownSystem();

    protected:
      void fFillMethodMetadata() override;

    private:
      NModuleManager mModuleManager;
      QMap<QString, QProcess*> mModulesRunning;  // ModuleNamespace--ModuleName--Version, Process
  };
}

#endif // NMainController_H
