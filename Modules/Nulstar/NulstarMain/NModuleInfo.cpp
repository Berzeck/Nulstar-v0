
#include "NModuleInfo.h"

namespace NulstarNS {
  NModuleInfo::NModuleInfo(const QString& lModuleName, const QString& lModuleNamespace, const QString& lModuleVersion, const QString& lModuleLanguage,
                           const QString& lModuleWorkingDirectory,  const QStringList& lModuleEnvLibPaths, const QList<NModuleParameter> &lModuleParameters)
              : mModuleName(lModuleName), mModuleLanguage(lModuleLanguage), mModuleNamespace(lModuleNamespace), mModuleVersion(lModuleVersion),
                mModuleWorkingDirectory(lModuleWorkingDirectory), mModuleEnvLibPaths(lModuleEnvLibPaths), mModuleParameters(lModuleParameters)  {

  }

  QStringList NModuleInfo::fFormattedParameters() const {
    QStringList lFormattedParameters;
    for(const NModuleParameter& lModuleParameter : mModuleParameters ) {
      QString lParameter(QString("--%1").arg(lModuleParameter.mParamName.toLower()));
      lFormattedParameters << lParameter << lModuleParameter.mParamValue;
    }
    return lFormattedParameters;
  }

  QString NModuleInfo::fModuleAppPath() const {
     QString lAppName(mModuleName);
#ifdef Q_OS_WIN
    lAppName.append(".exe");
#endif
    return QString("%1/%2").arg(mModuleWorkingDirectory).arg(lAppName);
  }

  bool NModuleInfo::fIsValid() const {
    if(mModuleName.isEmpty() || mModuleLanguage.isEmpty() || mModuleWorkingDirectory.isEmpty() || mModuleNamespace.isEmpty() || mModuleVersion.isEmpty())
      return false;
    return true;
  }

  QString NModuleInfo::fParameterValue(const QString& lParameter) const {
    for(const NModuleParameter& lModuleParameter : mModuleParameters ) {
       QString lCurrentParameter = lModuleParameter.mParamName.toLower();
       if(lParameter.toLower() == lCurrentParameter)
         return lModuleParameter.mParamValue;
    }
    return QString();
  }
}
