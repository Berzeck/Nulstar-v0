#ifndef NMAINCONTROLLER_H
#define NMAINCONTROLLER_H

#include "AppVersion.h"

#include <NCoreService.h>
#include <QMap>
#include <QObject>

#include "NModuleManager.h"

class QProcess;
const QString cServiceManager("ServiceManager");

namespace NulstarNS {
  class NMainController : public NCoreService {
    Q_OBJECT

    public:
      explicit NMainController();
      ~NMainController() override {}

      QString fName() const override { return QString(APP_NAME); }
      QString fVersion() const override { return QString(APP_VERSION); }
      QString fDomain() const override { return QString(APP_DOMAIN); }
      QString fApiRole() const override { return QString(APP_ROLE); }
      QString fApiVersion() const override { return QString(APP_ROLE_VERSION); }

    public Q_SLOTS:
      API_ADMIN_FUNCTION quint8 fScanManagedModules() { return mModuleManager.fLoadModulesInfo(); }      
      API_ADMIN_FUNCTION bool fStartModule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion = QString(), bool fRestartIfRunning = false);
      API_ADMIN_FUNCTION bool fStartAllModules(const QString& lModuleNamespace = QString(), bool fRestartIfRunning = false);
      API_ADMIN_FUNCTION bool fStopModule(const QString& lModuleNamespace, const QString& lModuleName, const QString& lModuleVersion = QString());
      API_ADMIN_FUNCTION bool fStopAllModules(const QString& lModuleNamespace = QString());
      API_ADMIN_FUNCTION void fShutdownSystem();

    private:
      NModuleManager mModuleManager;
      QMap<QString, QProcess*> mModulesRunning;  // ModuleNamespace--ModuleName--Version, Process
  };
}

#endif // NMainController_H
