#ifndef NVERSIONMANIFEST_H
#define NVERSIONMANIFEST_H

#include <QDate>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QVersionNumber>
#include "NTools.h"

namespace NulstarNS {
  const QString cParameterCategory_PackageSummary(QStringLiteral("PackageSummary"));
  const QString cParameterCategory_Sha256ExecHash(QStringLiteral("Sha256_ExecHash"));
  const QString cParameterCategory_Sha256LibHashes(QStringLiteral("Sha256_LibraryHashes"));
  const QString cParameterCategory_Sha256ModuleHashes(QStringLiteral("Sha256_ModuleHashes"));
  const QString cParameterCategory_Sha256PackageHash(QStringLiteral("Sha256_PackageHash"));
  const QString cParameterCategory_UpgradeLogs(QStringLiteral("UpgradeLogs"));
  const QString cParameterCategory_Validation(QStringLiteral("Validation"));
  const QString cParameterName_PackageName(QStringLiteral("Name"));
  const QString cParameterName_Platform(QStringLiteral("Platform"));
  const QString cParameterName_Priority(QStringLiteral("Priority"));
  const QString cParameterName_ReleaseDate(QStringLiteral("ReleaseDate"));
  const QString cParameterName_UpgradeNotes(QStringLiteral("UpgradeNotes"));
  const QString cParameterName_VersionName(QStringLiteral("VersionName"));
  const QString cParameterName_VersionNumber(QStringLiteral("VersionNumber"));
  const QString cParameterName_ValidationHashSignature(QStringLiteral("HashSignature"));
  const QString cParameterName_ValidationManifestHash(QStringLiteral("ManifestHash"));
  const QString cParameterName_ValidationPublicKey(QStringLiteral("PublicKey"));
  const QString cPrefix_ExecPackages(QStringLiteral("Exec"));
  const QString cPrefix_LibraryPackages(QStringLiteral("Library"));
  const QString cPrefix_ModulePackages(QStringLiteral("Module"));

  class NTOOLSSHARED_EXPORT  NVersionManifest : public QObject {
    Q_OBJECT

    public:
      explicit NVersionManifest(QObject* rParent = nullptr);

      bool fIsValid() const;
      bool fSetFile(const QString& lManifestPath);
      QString fHashSignature() const;
      QString fManifestHash() const;
      QString fPackageName() const;
      QString fFullPackageName() const;
      QString fPackageHash() const;
      QString fPlatform() const;
      QString fPriority() const;
      QDate fReleaseDate() const;
      QStringList fUpgradeLogs() const;
      QString fUpgradeNotes() const;
      QString fVersionName() const;
      QVersionNumber fVersionNumber() const;
      QVariantMap fExecHashes(bool lStripVersion = false) const;
      QVariantMap fLibraryHashes(bool lStripVersion = false) const;
      QVariantMap fModuleHashes(bool lStripVersion = false) const;
      QVariantMap fManifestContents() const;
      QVariantMap fPackagesToUpgrade(const NVersionManifest &lNewVersionManifest) const;

    private:
      QSettings* pVersionManifestFile;
  };
}

#endif // NVERSIONMANIFEST_H
