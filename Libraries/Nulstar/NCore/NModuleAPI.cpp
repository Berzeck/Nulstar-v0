#include <NMessage.h>
#include "Core.h"
#include "NCoreConstants.h"
#include "NModuleAPI.h"

namespace NulstarNS {
  NModuleAPI::NModuleAPI(const QVariantMap& lModuleApiMap)
            : QVariantMap(lModuleApiMap), mIsValid(false) {
    fFillFields(lModuleApiMap);
  }

  void NModuleAPI::fFillFields(const QVariantMap& lModuleApiMap) {
    if(!lModuleApiMap.contains(cFieldName_ModuleName))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_ModuleName)));
      return;
    }
    lModuleName = lModuleApiMap.value(cFieldName_ModuleName).toString();

    if(!lModuleApiMap.contains(cFieldName_Dependencies))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_Dependencies)));
      return;
    }
    lDependencies = lModuleApiMap.value(cFieldName_Dependencies).toMap();

    if(!lModuleApiMap.contains(cFieldName_IP))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_IP)));
      return;
    }
    lIP = QHostAddress(lModuleApiMap.value(cFieldName_IP).toString());
    if(lIP.isNull()) {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_IP)));
      return;
    }

    if(!lModuleApiMap.contains(cFieldName_Methods))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_Methods)));
      return;
    }
    lMethods = lModuleApiMap.value(cFieldName_Methods).toMap();

    if(!lModuleApiMap.contains(cFieldName_Abbreviation))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_Abbreviation)));
      return;
    }
    lModuleAbbreviation = lModuleApiMap.value(cFieldName_Abbreviation).toString();

    if(!lModuleApiMap.contains(cFieldName_ModuleDomain))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_ModuleDomain)));
      return;
    }
    lModuleDomain = lModuleApiMap.value(cFieldName_ModuleDomain).toString();

    if(!lModuleApiMap.contains(cFieldName_ModuleRoles))  {
      qDebug("%s", qUtf8Printable(QString("API from module '%1', field '%2' does not contain valid information!)").arg(cFieldName_ModuleName).arg(cFieldName_ModuleRoles)));
      return;
    }
    lModuleRoles = lModuleApiMap.value(cFieldName_ModuleRoles).toMap();

    if(!lModuleApiMap.contains(cFieldName_ModuleVersion))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_ModuleVersion)));
      return;
    }
    lModuleVersion = lModuleApiMap.value(cFieldName_ModuleVersion).toString();

    if(!lModuleApiMap.contains(cFieldName_Port))  {
      qDebug("%s", qUtf8Printable(QString("API from unknown module received! (Field '%1' does not contain valid information)").arg(cFieldName_Port)));
      return;
    }
    lPort = lModuleApiMap.value(cFieldName_Port).toString().toUShort();

    mIsValid = true;
  }
}
