#include <NMessage.h>
#include <QVersionNumber>
#include "NCoreConstants.h"
#include "NModuleAPI.h"

namespace NulstarNS {
  NModuleAPI::NModuleAPI()
            : mIsValid(false) {

  }

  NModuleAPI::NModuleAPI(const QVariantMap& lModuleApiMap)
            : mIsValid(false) {
    fFillFields(lModuleApiMap);
  }

  void NModuleAPI::fFillFields(const QVariantMap& lModuleApiMap) {
    if(!lModuleApiMap.contains(cFieldName_ModuleName))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_ModuleName)));
      return;
    }
    mModuleName = lModuleApiMap.value(cFieldName_ModuleName).toString();

    if(!lModuleApiMap.contains(cFieldName_Dependencies))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_Dependencies)));
      return;
    }
    mDependencies = lModuleApiMap.value(cFieldName_Dependencies).toMap();

    if(!lModuleApiMap.contains(cFieldName_IP))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_IP)));
      return;
    }
    mIP = QHostAddress(lModuleApiMap.value(cFieldName_ConnectionInformation).toMap().value(cFieldName_IP).toString());
    if(mIP.isNull()) {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_IP)));
      return;
    }

    if(!lModuleApiMap.contains(cFieldName_Methods))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_Methods)));
      return;
    }
    mMethods = lModuleApiMap.value(cFieldName_Methods).toMap();

    if(!lModuleApiMap.contains(cFieldName_Abbreviation))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_Abbreviation)));
      return;
    }
    mModuleAbbreviation = lModuleApiMap.value(cFieldName_Abbreviation).toString();

    if(!lModuleApiMap.contains(cFieldName_ModuleDomain))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_ModuleDomain)));
      return;
    }
    mModuleDomain = lModuleApiMap.value(cFieldName_ModuleDomain).toString();

    if(!lModuleApiMap.contains(cFieldName_ModuleRoles))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_ModuleRoles)));
      return;
    }
    QVariantMap lRoleMap = lModuleApiMap.value(cFieldName_ModuleRoles).toMap();
    QMapIterator<QString, QVariant> i1(lRoleMap);
    while(i1.hasNext()) {
      i1.next();
      NModuleAPIRole lAPIRole;
      lAPIRole.fSetRoleName(i1.key());
      lAPIRole.fSetVersionNumber(QVersionNumber::fromString(i1.value().toString()));
      mModuleRoles << lAPIRole;
  //    mModuleRoles << NModuleAPIRole(i1.key(), QVersionNumber::fromString(i1.value().toString()));
    }

    if(!lModuleApiMap.contains(cFieldName_ModuleVersion))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_ModuleVersion)));
      return;
    }
    mModuleVersion = lModuleApiMap.value(cFieldName_ModuleVersion).toString();

    if(!lModuleApiMap.contains(cFieldName_Port))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_Port)));
      return;
    }
    mPort = lModuleApiMap.value(cFieldName_Port).toString().toUShort();

    mIsValid = true;
  }

 /* bool NModuleAPI::fIsRoleSupported(const QString& lRoleName, const QString& lVersion) const {
    if(mModuleRoles.contains(lRoleName)) {
      if(mModuleRoles[lRoleName].toList().contains(lVersion))
        return true;
      else {
        qDebug("%s", qUtf8Printable(QString("Version '%1' of role '%2' requiered by module '%3' not supported!").arg(lVersion).arg(lRoleName).arg(fModuleName())));
        return false;
      }
    }
    return false;
  }*/
}
