#include <QFile>

#include "NVersionManifest.h"

namespace NulstarNS {
  NVersionManifest::NVersionManifest(QObject *parent) : QObject(parent) {
    pVersionManifestFile = nullptr;
  }

  bool NVersionManifest::fSetFile(const QString& lManifestPath) {
    if(pVersionManifestFile != nullptr) {
      pVersionManifestFile->deleteLater();
      pVersionManifestFile = nullptr;
    }
    if(QFile::exists(lManifestPath)) {
      pVersionManifestFile = new QSettings(lManifestPath, QSettings::IniFormat, this);
      return true;
    }
    return false;
  }

  bool NVersionManifest::fIsValid() const {
    if(pVersionManifestFile == nullptr)
      return false;
    return true;
  }

  QString NVersionManifest::fHashSignature() const {
    if(!fIsValid())
      return QString();
    QString lHashSignature(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_Validation).arg(cParameterName_ValidationHashSignature)).toString());
    return lHashSignature;
  }

  QString NVersionManifest::fManifestHash() const {
    if(!fIsValid())
      return QString();
    QString lManifestHash(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_Validation).arg(cParameterName_ValidationManifestHash)).toString());
    return lManifestHash;
  }

  QString NVersionManifest::fPackageName() const {
    if(!fIsValid())
      return QString();
    QString lPackageName(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_PackageSummary).arg(cParameterName_PackageName)).toString());
    return lPackageName;
  }

  QString NVersionManifest::fFullPackageName() const {
    if(!fIsValid())
      return QString();
    QString lFullPackageName(QString("%1_%2_%3").arg(fPackageName()).arg(fPlatform()).arg(fVersionNumber().toString()));
    return lFullPackageName;
  }

  QString NVersionManifest::fPackageHash() const {
    if(!fIsValid())
      return QString();
    pVersionManifestFile->beginGroup(cParameterCategory_Sha256PackageHash);
    QStringList lPackageHashKeys(pVersionManifestFile->childKeys());
    QString lPackageKey(lPackageHashKeys.isEmpty() ? QString() : lPackageHashKeys[0]);
    QString lPackageHash;
    if(!lPackageKey.isEmpty())
      lPackageHash = pVersionManifestFile->value(lPackageKey).toString();
    pVersionManifestFile->endGroup();
    return lPackageHash;
  }

  QString NVersionManifest::fPlatform() const {
    if(!fIsValid())
      return QString();
    QString lPlatform(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_PackageSummary).arg(cParameterName_Platform)).toString());
    return lPlatform;
  }

  QString NVersionManifest::fPriority() const {
    if(!fIsValid())
      return QString();
    QString lPriority(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_PackageSummary).arg(cParameterName_Priority)).toString());
    return lPriority;
  }

  QDate NVersionManifest::fReleaseDate() const {
    if(!fIsValid())
      return QDate();
    QString lReleaseDate(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_PackageSummary).arg(cParameterName_ReleaseDate)).toString());
    return QDate::fromString(lReleaseDate, Qt::ISODate);
  }

  QStringList NVersionManifest::fUpgradeLogs() const {
    if(!fIsValid())
      return QStringList();
    QStringList lUpgradeLogs;
    pVersionManifestFile->beginGroup(cParameterCategory_UpgradeLogs);
    QStringList lUpgradeLogsKeys(pVersionManifestFile->childKeys());
    for(const QString& lKey : lUpgradeLogsKeys) {
      lUpgradeLogs.append(pVersionManifestFile->value(QString(lKey)).toString());
    }
    pVersionManifestFile->endGroup();
    return lUpgradeLogs;
  }

  QString NVersionManifest::fUpgradeNotes() const {
    if(!fIsValid())
      return QString();
    QString lUpgradeNotes(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_PackageSummary).arg(cParameterName_UpgradeNotes)).toString());
    return lUpgradeNotes;
  }

  QString NVersionManifest::fVersionName() const {
    if(!fIsValid())
      return QString();
    QString lVersionName(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_PackageSummary).arg(cParameterName_VersionName)).toString());
    return lVersionName;
  }

  QVersionNumber NVersionManifest::fVersionNumber() const {
    if(!fIsValid())
      return QVersionNumber(0,0,0);
    QString lVersionNumber(pVersionManifestFile->value(QString("%1/%2").arg(cParameterCategory_PackageSummary).arg(cParameterName_VersionNumber)).toString());
    return QVersionNumber(QVersionNumber::fromString(lVersionNumber));
  }

  QVariantMap NVersionManifest::fExecHashes() const {
    if(!fIsValid())
      return QVariantMap();
    QVariantMap lExecHashes;
    pVersionManifestFile->beginGroup(cParameterCategory_Sha256ExecHash);
    QStringList lExecKeys(pVersionManifestFile->childKeys());
    for(const QString& lKey : lExecKeys) {
      lExecHashes[lKey] = pVersionManifestFile->value(QString(lKey)).toString();
    }
    pVersionManifestFile->endGroup();
    return lExecHashes;
  }

  QVariantMap NVersionManifest::fLibraryHashes() const {
    if(!fIsValid())
      return QVariantMap();
    QVariantMap lLibHashes;
    pVersionManifestFile->beginGroup(cParameterCategory_Sha256LibHashes);
    QStringList lLibKeys(pVersionManifestFile->childKeys());
    for(const QString& lKey : lLibKeys) {
      lLibHashes[lKey] = pVersionManifestFile->value(QString(lKey)).toString();
    }
    pVersionManifestFile->endGroup();
    return lLibHashes;
  }

  QVariantMap NVersionManifest::fModuleHashes() const {
    if(!fIsValid())
      return QVariantMap();
    QVariantMap lModuleHashes;
    pVersionManifestFile->beginGroup(cParameterCategory_Sha256ModuleHashes);
    QStringList lModuleKeys(pVersionManifestFile->childKeys());
    for(const QString& lKey : lModuleKeys) {
      lModuleHashes[lKey] = pVersionManifestFile->value(QString(lKey)).toString();
    }
    pVersionManifestFile->endGroup();
    return lModuleHashes;
  }

  QVariantMap NVersionManifest::fManifestContents() const {
      // Package Summary
    QVariantMap lPackageSummary;
    lPackageSummary[cParameterName_PackageName] = fPackageName();
    lPackageSummary[cParameterName_Platform] = fPlatform();
    lPackageSummary[cParameterName_Priority] = fPriority();
    lPackageSummary[cParameterName_ReleaseDate] = fReleaseDate().toString(Qt::ISODate);
    lPackageSummary[cParameterName_UpgradeNotes] = fUpgradeNotes();
    lPackageSummary[cParameterName_VersionName] = fVersionName();
    lPackageSummary[cParameterName_VersionNumber] = fVersionNumber().toString();

        // Validation
    QVariantMap lValidation;
    lValidation[cParameterName_ValidationHashSignature] = fHashSignature();
    lValidation[cParameterName_ValidationManifestHash] = fManifestHash();
      //lValidation[cParameterName_ValidationPublicKey] = fHashSignature(); // Should be embedded

        // Manifest
    QVariantMap lManifest;
    lManifest[cParameterCategory_PackageSummary] = lPackageSummary;
    lManifest[cParameterCategory_Sha256ExecHash] = fExecHashes();
    lManifest[cParameterCategory_Sha256LibHashes] = fLibraryHashes();
    lManifest[cParameterCategory_Sha256ModuleHashes] = fModuleHashes();
    lManifest[cParameterCategory_Sha256PackageHash] = fPackageHash();
    lManifest[cParameterCategory_UpgradeLogs] = fUpgradeLogs();
    lManifest[cParameterCategory_Validation] = lValidation;

    return lManifest;
  }
}
