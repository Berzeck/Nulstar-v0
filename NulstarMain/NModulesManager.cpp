#include <QCoreApplication>
#include <QHostAddress>
#include <QProcess>
#include <QTimer>
#include <QWebSocket>
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
       QString lModulesDir =  QString("%1/%2/").arg(QCoreApplication::applicationDirPath()).arg(lModulesDirectory);
    }


}
