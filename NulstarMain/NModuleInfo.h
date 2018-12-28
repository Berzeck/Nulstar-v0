#ifndef NMODULEINFO_H
#define NMODULEINFO_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>

namespace NulstarNS {
  class NModuleInfo {
    public:
      explicit NModuleInfo(const QString& lModuleAppName, const QString& lModuleLanguage = QString(), const QString& lModuleWorkingDirectory = QString(),
                           const QStringList& lModuleEnvLibPaths = QStringList(), const QMap<QString, QString>& lModuleParameters = QMap<QString, QString>());
      ~NModuleInfo() { }

      bool fIsValid() const;
      QString fModuleAppName() const { return mModuleAppName; }
      QString fModuleLanguage() const { return mModuleLanguage; }
      QString fModuleWorkingDirectory() const { return mModuleWorkingDirectory; }
      QStringList fModuleEnvLibPaths() const { return mModuleEnvLibPaths; }
      QMap<QString, QString> fModuleParameters() const { return mModuleParameters; }

      QString fModuleExecutablePath() const { return QString("%1/%2").arg(mModuleWorkingDirectory).arg(mModuleAppName); }
      QString fParameterValue(const QString& lParameter) const;
      QStringList fFormattedParameters() const;
      QString fLibPaths() const { return mModuleEnvLibPaths.join(";"); }

      void fSetModuleLanguage(const QString& lModuleLanguage) { mModuleLanguage = lModuleLanguage; }
      void fSetModuleWorkingDirectory(const QString& lModuleWorkingDirectory) { mModuleWorkingDirectory = lModuleWorkingDirectory; }
      void fSetModuleEnvLibPaths(const QStringList& lModuleEnvLibPaths) { mModuleEnvLibPaths = lModuleEnvLibPaths; }
      void fSetModuleParameters(const QMap<QString, QString>& lModuleParameters) { mModuleParameters = lModuleParameters; }

    private:
      QString mModuleAppName;
      QString mModuleLanguage;
      QString mModuleWorkingDirectory;
      QStringList mModuleEnvLibPaths;
      QMap<QString, QString> mModuleParameters;
  };
}

#endif // NMODULEINFO_H
