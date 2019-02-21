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
      API_ADMIN_FUNCTION quint8 scanmanagedmodules();
      API_ADMIN_FUNCTION bool startmodule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion = QString(), bool fRestartIfRunning = false);
      API_ADMIN_FUNCTION bool startallmodules(const QString& lModuleNamespace = QString(), bool fRestartIfRunning = false);
      API_ADMIN_FUNCTION bool stopmodule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion = QString());
      API_ADMIN_FUNCTION bool stopallmodules(const QString& lModuleNamespace = QString());
      API_ADMIN_FUNCTION void shutdownsystem();

    protected:
      void fFillMethodMetadata() override;

    private:
      NModuleManager mModuleManager;
      QMap<QString, QProcess*> mModulesRunning;  // ModuleNamespace--ModuleName--Version, Process
  };
}

#endif // NMainController_H
