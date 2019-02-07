#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QString>
#include "NApiBuilder.h"
#include "NCoreService.h"

namespace NulstarNS {
  const QString cFieldName_Dependencies(QStringLiteral("Dependencies"));
  const QString cFieldName_IP(QStringLiteral("IP"));
  const QString cFieldName_ModuleName(QStringLiteral("ModuleName"));
  const QString cFieldName_ModuleDomain(QStringLiteral("ModuleDomain"));
  const QString cFieldName_ModuleRoles(QStringLiteral("ModuleRoles"));
  const QString cFieldName_ModuleVersion(QStringLiteral("ModuleVersion"));
  const QString cFieldName_ConnectionInformation(QStringLiteral("ConnectionInformation"));
  const QString cFieldName_Port(QStringLiteral("Port"));
  const QString cFieldName_Abbreviation(QStringLiteral("Abbreviation"));

  const QString cFieldName_MethodName(QStringLiteral("MethodName"));
  const QString cFieldName_MethodScope(QStringLiteral("MethodScope"));
  const QString cFieldName_MethodDescription(QStringLiteral("MethodDescription"));
  const QString cFieldName_MethodMinEvent(QStringLiteral("MethodMinEvent"));
  const QString cFieldName_MethodMinPeriod(QStringLiteral("MethodMinPeriod"));
  const QString cFieldName_ParameterName(QStringLiteral("ParameterName"));
  const QString cFieldName_ParameterType(QStringLiteral("ParameterType"));
  const QString cFieldName_Parameters(QStringLiteral("Parameters"));
  const QString cFieldName_ParameterValidRange(QStringLiteral("ParameterValidRange"));
  const QString cFieldName_ParameterValidRegExp(QStringLiteral("ParameterValidRegExp"));
  const QString cFieldName_Methods(QStringLiteral("Methods"));
  const QString cFieldName_RegisterAPI(QStringLiteral("RegisterAPI"));

  const QString cFunctionTag_Admin(QStringLiteral("API_ADMIN_FUNCTION"));
  const QString cFunctionTag_Public(QStringLiteral("API_PUBLIC_FUNCTION"));
  const QString cFunctionTag_Private(QStringLiteral("API_PRIVATE_FUNCTION"));

  NApiBuilder::NApiBuilder(QObject* rParent)
             : QObject(rParent) {

  }

  void NApiBuilder::fBuildApi(NCoreService *rTargetObject) {
    mApi.clear();
    if(rTargetObject != nullptr) {
      QVariantMap lApiMap;
      const QMetaObject* lMetaObject = rTargetObject->metaObject();
      lApiMap = fBuildApiHeader(rTargetObject);
      QVariantList lMethods;
      QMap<QString, int> lParamCount; // Methods with default parameters are counted more than once so we choose the method with most parameters
      for(int i = lMetaObject->methodOffset(); i < lMetaObject->methodCount(); ++i) {
        QVariantMap lMethodDetail;
        QMetaMethod lApiMethod = rTargetObject->metaObject()->method(i);
        if((QString(lApiMethod.tag()) == cFunctionTag_Admin) || (QString(lApiMethod.tag()) == cFunctionTag_Private) || (QString(lApiMethod.tag()) == cFunctionTag_Public))  {
          QString lApiMethodName(lApiMethod.name());
          if(rTargetObject->fApiMethodLowercase())
            lApiMethodName = lApiMethodName.toLower();
          if(rTargetObject->fApiMethodNameOffset())
            lApiMethodName = lApiMethodName.right(lApiMethodName.size() - 1);

          if(!lParamCount.contains(lApiMethodName) || (lParamCount.value(lApiMethodName) < lApiMethod.parameterCount())) {
            lParamCount[lApiMethodName] = lApiMethod.parameterCount();
            lMethodDetail[cFieldName_MethodName] = lApiMethodName;
            lMethodDetail[cFieldName_MethodScope] = QString(lApiMethod.tag()).section("_",1,1).toLower();
            lMethodDetail[cFieldName_MethodDescription] = rTargetObject->fMethodDescription(lApiMethod.name());
            lMethodDetail[cFieldName_MethodMinEvent] = rTargetObject->fMethodMinEventAndMinPeriod(lApiMethod.name()).split(",").at(0).simplified();
            lMethodDetail[cFieldName_MethodMinPeriod] = rTargetObject->fMethodMinEventAndMinPeriod(lApiMethod.name()).split(",").at(1).simplified();

            QList<QVariant> lParameters;
            for(int j = 0; j < lApiMethod.parameterCount(); ++j) {
              QMap<QString, QVariant> lParameterDetail;
              lParameterDetail[cFieldName_ParameterName] = QString(lApiMethod.parameterNames().at(j));
              lParameterDetail[cFieldName_ParameterType] = QString(lApiMethod.parameterTypes().at(j));
              lParameterDetail[cFieldName_ParameterValidRange] = QString();
              lParameterDetail[cFieldName_ParameterValidRegExp] = QString();
              lParameters << lParameterDetail;
            }
            lMethodDetail[cFieldName_Parameters] = lParameters;
            lMethods << lMethodDetail;
          }
        }
      }
      lApiMap[cFieldName_Methods] = lMethods;
      mApi[cFieldName_RegisterAPI] = lApiMap;
    }    
  }

  QVariantMap NApiBuilder::fBuildApiHeader(NCoreService *pTargetObject) {
    QVariantMap lApiHeader;
    if(pTargetObject != nullptr) {
      lApiHeader[cFieldName_Dependencies] = pTargetObject->fDependencies();
      lApiHeader[cFieldName_ConnectionInformation] = QVariantMap( {{cFieldName_IP, pTargetObject->fHostAddress().toString()}, {cFieldName_Port, QString::number(pTargetObject->fCommPort()) } } ) ;
      lApiHeader[cFieldName_ModuleDomain] = pTargetObject->fDomain();
      lApiHeader[cFieldName_ModuleName] = pTargetObject->fName();
      lApiHeader[cFieldName_Abbreviation] = pTargetObject->fAbbreviation();
      lApiHeader[cFieldName_ModuleVersion] = pTargetObject->fVersion();
      lApiHeader[cFieldName_ModuleRoles] = pTargetObject->fApiRoles();
    }
    return lApiHeader;
  }
}
