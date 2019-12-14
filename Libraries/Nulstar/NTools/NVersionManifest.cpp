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
    else
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

  QMap<QString, QString> NVersionManifest::fExecHashes() const {
    if(!fIsValid())
      return QMap<QString, QString>();
    QMap<QString, QString> lExecHashes;
    pVersionManifestFile->beginGroup(cParameterCategory_Sha256ExecHash);
    QStringList lExecKeys(pVersionManifestFile->childKeys());
    for(const QString& lKey : lExecKeys) {
      lExecHashes[lKey] = pVersionManifestFile->value(QString(lKey)).toString();
    }
    pVersionManifestFile->endGroup();
    return lExecHashes;
  }

  QMap<QString, QString> NVersionManifest::fLibraryHashes() const {
    if(!fIsValid())
      return QMap<QString, QString>();
    QMap<QString, QString> lLibHashes;
    pVersionManifestFile->beginGroup(cParameterCategory_Sha256LibHashes);
    QStringList lLibKeys(pVersionManifestFile->childKeys());
    for(const QString& lKey : lLibKeys) {
      lLibHashes[lKey] = pVersionManifestFile->value(QString(lKey)).toString();
    }
    pVersionManifestFile->endGroup();
    return lLibHashes;
  }

  QMap<QString, QString> NVersionManifest::fModuleHashes() const {
    if(!fIsValid())
      return QMap<QString, QString>();
    QMap<QString, QString> lModuleHashes;
    pVersionManifestFile->beginGroup(cParameterCategory_Sha256ModuleHashes);
    QStringList lModuleKeys(pVersionManifestFile->childKeys());
    for(const QString& lKey : lModuleKeys) {
      lModuleHashes[lKey] = pVersionManifestFile->value(QString(lKey)).toString();
    }
    pVersionManifestFile->endGroup();
    return lModuleHashes;
  }
}
