#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QString>
#include "NApiBuilder.h"
#include "NCoreService.h"

namespace NulstarNS {
  const QString lApiPeriodField("Period");
  const QString lApiEventField("Event");
  const QString lApiParametersField("Parameters");
  const QString lApiParameterNameField("ParameterName");
  const QString lApiParameterTypeField("ParameterType");
  const QString lApiMethodsField("Methods");
  const QString lApiMethodNameField("MethodName");
  const QString lApiMethodScopeField("MethodScope");  
  const QString lApiMethodDescriptionField("MethodDescription");
  const QString lApiMethodMinEventField("MethodMinEvent");
  const QString lApiMethodMinPeriodField("MethodMinPeriod");
  const QString lApiAdminFunctionMacro("API_ADMIN_FUNCTION");
  const QString lApiPublicFunctionMacro("API_PUBLIC_FUNCTION");
  const QString lApiPrivateFunctionMacro("API_PRIVATE_FUNCTION");
  const QString lRegisterApiField("RegisterAPI");
  const QString lServiceNameFieldName("ServiceName");
  const QString lServiceVersionFieldName("ServiceVersion");
  const QString lServiceDomainFieldName("ServiceDomain");
  const QString lServiceRoleFieldName("ServiceRole");
  const QString lServiceApiVersionFieldName("ServiceAPIVersion");  

  NApiBuilder::NApiBuilder(QObject* rParent)
             : QObject(rParent) {

  }

  void NApiBuilder::fBuildApi(NCoreService *rTargetObject) {
    mApi.clear();
    if(rTargetObject != nullptr) {
      QMap<QString, QVariant> lApiMap;
      const QMetaObject* lMetaObject = rTargetObject->metaObject();
      lApiMap = fBuildApiHeader(rTargetObject);
      QList<QVariant> lMethods;
      for(int i = lMetaObject->methodOffset(); i < lMetaObject->methodCount(); ++i) {
        QMap<QString, QVariant> lMethodDetail;
        QMetaMethod lApiMethod = rTargetObject->metaObject()->method(i);
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
          lMethodDetail[lApiMethodDescriptionField] = rTargetObject->fMethodDescription(lApiMethod.name());
          lMethodDetail[lApiMethodMinEventField] = rTargetObject->fMethodMinEventAndMinPeriod(lApiMethod.name()).split(",").at(0).simplified();
          lMethodDetail[lApiMethodMinPeriodField] = rTargetObject->fMethodMinEventAndMinPeriod(lApiMethod.name()).split(",").at(1).simplified();
          lMethods << lMethodDetail;
        }        
      }
      lApiMap[lApiMethodsField] = lMethods;
      lApiMap[lApiPeriodField] = QString("0");
      lApiMap[lApiEventField] = QString("0");
      mApi[lRegisterApiField] = lApiMap;
    }    
  }

  QVariantMap NApiBuilder::fBuildApiHeader(NCoreService *pTargetObject) {
    QMap<QString, QVariant> lApiHeader;
    if(pTargetObject != nullptr) {
      lApiHeader[lServiceNameFieldName] = pTargetObject->fName();
      lApiHeader[lServiceVersionFieldName] = pTargetObject->fVersion();
      lApiHeader[lServiceDomainFieldName] = pTargetObject->fDomain();
      lApiHeader[lServiceRoleFieldName] = pTargetObject->fApiRole();
      //lApiHeader[lServiceApiVersionFieldName] = pTargetObject->fApiVersion();
    }
    return lApiHeader;
  }
}
