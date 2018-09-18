#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QString>
#include "NApiBuilder.h"
#include "NCoreService.h"

namespace NulstarNS {
  const QString lApiParametersField("Parameters");
  const QString lApiParameterNameField("ParameterName");
  const QString lApiParameterTypeField("ParameterType");
  const QString lApiMethodsField("Methods");
  const QString lApiMethodNameField("MethodName");
  const QString lApiMethodScopeField("MethodScope");
  const QString lApiAdminFunctionMacro("API_ADMIN_FUNCTION");
  const QString lApiPublicFunctionMacro("API_PUBLIC_FUNCTION");
  const QString lApiPrivateFunctionMacro("API_PRIVATE_FUNCTION");

  const QString lServiceNameFieldName("ServiceName");
  const QString lServiceVersionFieldName("ServiceVersion");
  const QString lServiceDomainFieldName("ServiceDomain");
  const QString lServiceRoleFieldName("ServiceRole");
  const QString lServiceApiVersionFieldName("ServiceAPIVersion");

  NApiBuilder::NApiBuilder(QObject* rParent)
             : QObject(rParent) {

  }

  QMap<QString, QVariant> NApiBuilder::fBuildApi() {
    QMap<QString, QVariant> lApiMap;
    if(pTargetObject != nullptr) {
      const QMetaObject* lMetaObject = pTargetObject->metaObject();
      lApiMap = fExtractHeader();
      QList<QVariant> lMethods;
      for(int i = lMetaObject->methodOffset(); i < lMetaObject->methodCount(); ++i) {
        QMap<QString, QVariant> lMethodDetail;
        QMetaMethod lApiMethod = pTargetObject->metaObject()->method(i);
        lMethodDetail[lApiMethodNameField] = lApiMethod.name();
        if((QString(lApiMethod.tag()) == lApiAdminFunctionMacro) || (QString(lApiMethod.tag()) == lApiPrivateFunctionMacro) || (QString(lApiMethod.tag()) == lApiPublicFunctionMacro))  {
          lMethodDetail[lApiMethodScopeField] = QString(lApiMethod.tag()).section("_",1,1).toLower();
          QList<QVariant> lParameters;
          for(int j = 0; j < lApiMethod.parameterCount(); ++j) {
            QMap<QString, QVariant> lParameterDetail;
            lParameterDetail[lApiParameterNameField] = QString(lApiMethod.parameterNames().at(j));
            lParameterDetail[lApiParameterTypeField] = QString(lApiMethod.parameterTypes().at(j));
            lParameters << lParameterDetail;
          }
          lMethodDetail[lApiParametersField] = lParameters;
        }
        lMethods << lMethodDetail;
      }
      lApiMap[lApiMethodsField] = lMethods;
    }
    return lApiMap;
  }

  QMap<QString, QVariant> NApiBuilder::fExtractHeader() {
    QMap<QString, QVariant> lApiHeader;
    if(pTargetObject != nullptr) {
      lApiHeader[lServiceNameFieldName] = pTargetObject->fName();
      lApiHeader[lServiceVersionFieldName] = pTargetObject->fVersion();
      lApiHeader[lServiceDomainFieldName] = pTargetObject->fDomain();
      lApiHeader[lServiceRoleFieldName] = pTargetObject->fApiRole();
      lApiHeader[lServiceApiVersionFieldName] = pTargetObject->fApiVersion();
    }
    return lApiHeader;
  }
}
