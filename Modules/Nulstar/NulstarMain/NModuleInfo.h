#ifndef NMODULEINFO_H
#define NMODULEINFO_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>

namespace NulstarNS {
  class SModuleParameter {
    public:
        QString mGroupName;
        QString mParamName;
        QString mParamValue;
        inline SModuleParameter &operator=(const SModuleParameter& lModuleParam) {
            mGroupName = lModuleParam.mGroupName;
            mParamName = lModuleParam.mParamName;
            mParamValue = lModuleParam.mParamValue;
            return *this;
        }

        inline bool operator==(const SModuleParameter& lModuleParam) {
            return (mGroupName == lModuleParam.mGroupName) && (mParamName == lModuleParam.mParamName) && (mParamValue == lModuleParam.mParamValue);
        }
  };

  class NModuleInfo {
    public:
      explicit NModuleInfo(const QString& lModulName, const QString &lModuleNamespace, const QString &lModuleVersion,  const QString& lModuleLanguage, const QString& lModuleWorkingDirectory,
                           const QStringList& lModuleEnvLibPaths, const QList<SModuleParameter>& lModuleParameters);
      ~NModuleInfo() { }

      bool fIsValid() const;
      QString fModuleAppPath() const;
      QString fModuleName() const { return mModuleName; }
      QString fModuleVersion() const { return mModuleVersion; }
      QString fModuleNamespace() const { return mModuleNamespace; }
      QString fModuleLanguage() const { return mModuleLanguage; }
      QString fModuleWorkingDirectory() const { return mModuleWorkingDirectory; }
      QStringList fModuleEnvLibPaths() const { return mModuleEnvLibPaths; }
      QList<SModuleParameter> fModuleParameters() const { return mModuleParameters; }

      QString fParameterValue(const QString& lParameter) const;
      QStringList fFormattedParameters() const;
      QString fLibPaths() const { return mModuleEnvLibPaths.join(";"); }

    private:
      QString mModuleName;
      QString mModuleLanguage;
      QString mModuleNamespace;
      QString mModuleVersion;
      QString mModuleWorkingDirectory;
      QStringList mModuleEnvLibPaths;
      QList<SModuleParameter> mModuleParameters;
  };
}

#endif // NMODULEINFO_H
