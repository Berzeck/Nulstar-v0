
#include "NModuleInfo.h"

namespace NulstarNS {
  NModuleInfo::NModuleInfo(const QString& lModuleAppName, const QString& lModuleLanguage, const QString& lModuleWorkingDirectory,
                           const QStringList& lModuleEnvLibPaths, const QMap<QString, QString>& lModuleParameters)
              : mModuleAppName(lModuleAppName), mModuleLanguage(lModuleLanguage), mModuleWorkingDirectory(lModuleWorkingDirectory),
                mModuleEnvLibPaths(lModuleEnvLibPaths), mModuleParameters(lModuleParameters)  {

  }

  QStringList NModuleInfo::fFormattedParameters() const {
    QStringList lFormattedParameters;
    QMapIterator<QString, QString> i(mModuleParameters);
    while(i.hasNext()) {
      i.next();
      QString lParameter(QString("--%1 %2").arg(i.key().toLower()).arg(i.value()));
      lFormattedParameters << lParameter;
    }
    return lFormattedParameters;
  }

  bool NModuleInfo::fIsValid() const {
    if(mModuleAppName.isEmpty() || mModuleLanguage.isEmpty() || mModuleWorkingDirectory.isEmpty())
      return false;
    return true;
  }

  QString NModuleInfo::fParameterValue(const QString& lParameter) const {
    QString lParameterValue;
    if(mModuleParameters.contains(lParameter.toLower()))
       lParameterValue = mModuleParameters.value(lParameter);
    return lParameterValue;
  }
}
