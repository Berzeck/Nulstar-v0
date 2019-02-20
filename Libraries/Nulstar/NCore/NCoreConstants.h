#ifndef NCORECONSTANTS_H
#define NCORECONSTANTS_H

#include <QString>

namespace NulstarNS {
  const int cRetryInterval = 5; // Seconds

  const QString cRole_ServiceManager(QStringLiteral("Role_ServiceManager"));
  const QString cCommServerName(QStringLiteral("WebCommServer"));
  const QString cConstantsFile("Constants.ncf");
  const QString cCommServerLabel("Nulstar Internal Communication");
  const QString cDefaultMinEventAndMinPeriod("0,0");
  const QString cServiceManagerName("ServiceManager") ;
  const QString cVersion_ServiceManagerRole(QStringLiteral("0.1"));

  const QString cFieldName_Dependencies(QStringLiteral("Dependencies"));
  const QString cFieldName_IP(QStringLiteral("IP"));
  const QString cFieldName_ModuleName(QStringLiteral("ModuleName"));
  const QString cFieldName_ModuleDomain(QStringLiteral("ModuleDomain"));
  const QString cFieldName_ModuleRoles(QStringLiteral("ModuleRoles"));
  const QString cFieldName_ModuleRoleVersion(QStringLiteral("APIVersion"));
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
}

#endif // NCORECONSTANTS_H
