#include <QDebug>
#include "NModuleInfo.h"

namespace NulstarNS {
  const QString cModuleConfigGroupScript("Script");
  const QString cModuleConfigGroupScripted("Scripted");
  const QString cModuleConfigGroupScriptStart("StartScript");
  const QString cModuleConfigGroupLibJre("JRE");

  NModuleInfo::NModuleInfo(const QString& lModuleName, const QString& lModuleNamespace, const QString& lModuleVersion, const QString& lModuleLanguage,
                           const QString& lModuleWorkingDirectory,  const QStringList& lModuleEnvLibPaths, const QList<NModuleParameter> &lModuleParameters, const QList<NModuleParameter> &lModuleScriptParameters)
              : mModuleName(lModuleName), mModuleLanguage(lModuleLanguage), mModuleNamespace(lModuleNamespace), mModuleVersion(lModuleVersion),
                mModuleWorkingDirectory(lModuleWorkingDirectory), mModuleEnvLibPaths(lModuleEnvLibPaths), mModuleParameters(lModuleParameters), mModuleScriptParameters(lModuleScriptParameters)   {

  }

  QStringList NModuleInfo::fFormattedParameters() const {
    QStringList lFormattedParameters;
    QString lParamValue;
    for(const NModuleParameter& lModuleParameter : mModuleParameters ) {
      QString lParameter(QString("--%1").arg(lModuleParameter.mParamName.toLower()));
      lParamValue = lModuleParameter.mParamValue;
 #ifdef Q_OS_WIN
      if (lModuleParameter.mParamName.toLower() == cModuleConfigGroupLibJre.toLower()){
        lParamValue.replace("/", "\\", Qt::CaseInsensitive);
      }
 #endif
      lFormattedParameters << lParameter << lParamValue;
    }
    return lFormattedParameters;
  }

  QString NModuleInfo::fModuleAppPath() const {
    if(bool(fScriptParameterValue(cModuleConfigGroupScripted) == QString("%1").arg(1))){
      QString lStartScriptName(fScriptParameterValue(cModuleConfigGroupScriptStart));
#ifdef Q_OS_WIN
      lStartScriptName.append(".bat");
#else
      lStartScriptName.append(".sh");
#endif
      return QString("%1/%2").arg(mModuleWorkingDirectory).arg(lStartScriptName);
    }

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
  QString NModuleInfo::fScriptParameterValue(const QString& lParameter) const {
    for(const NModuleParameter& lScriptModuleParameter : mModuleScriptParameters ) {
       QString lCurrentParameter = lScriptModuleParameter.mParamName.toLower();
       if(lParameter.toLower() == lCurrentParameter)
         return lScriptModuleParameter.mParamValue;
    }
    return QString();
  }
}
