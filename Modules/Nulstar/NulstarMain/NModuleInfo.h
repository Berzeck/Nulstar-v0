#ifndef NMODULEINFO_H
#define NMODULEINFO_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include "NModuleParameter.h"

namespace NulstarNS {
  class NModuleInfo {
    public:
      explicit NModuleInfo(const QString& lModulName, const QString &lModuleNamespace, const QString &lModuleVersion,  const QString& lModuleLanguage, const QString& lModuleWorkingDirectory,
                           const QStringList& lModuleEnvLibPaths, const QList<NModuleParameter>& lModuleParameters, const QList<NModuleParameter>& lModuleScriptParameters);
      ~NModuleInfo() { }

      bool fIsValid() const;
      QString fModuleAppPath() const;
      QString fModuleName() const { return mModuleName; }
      QString fModuleVersion() const { return mModuleVersion; }
      QString fModuleNamespace() const { return mModuleNamespace; }
      QString fModuleLanguage() const { return mModuleLanguage; }
      QString fModuleWorkingDirectory() const { return mModuleWorkingDirectory; }
      QStringList fModuleEnvLibPaths() const { return mModuleEnvLibPaths; }
      QList<NModuleParameter> fModuleParameters() const { return mModuleParameters; }

      QString fParameterValue(const QString& lParameter) const;
      QStringList fFormattedParameters() const;
      QString fLibPaths() const { return mModuleEnvLibPaths.join(";"); }
      QString fScriptParameterValue(const QString& lParameter) const;
    private:
      QString mModuleName;
      QString mModuleLanguage;
      QString mModuleNamespace;
      QString mModuleVersion;
      QString mModuleWorkingDirectory;
      QStringList mModuleEnvLibPaths;
      QList<NModuleParameter> mModuleParameters;
      QList<NModuleParameter> mModuleScriptParameters;
  };
}

#endif // NMODULEINFO_H
