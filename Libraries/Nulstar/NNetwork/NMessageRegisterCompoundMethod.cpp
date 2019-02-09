#include "NMessageRegisterCompoundMethod.h"

namespace NulstarNS {
  const QString lCompoundMethodNameFieldName("CompoundMethodName");
  const QString lCompoundMethodDescriptionFieldName("CompoundMethodDescription");
  const QString lCompoundMethodsFieldName("CompoundMethods");

  NMessageRegisterCompoundMethod::NMessageRegisterCompoundMethod(const QString& lConnectionName, const QString& lMessageID, const QString& lCompoundMethodName, const QString& lCompoundMethodDescription, const QVariantMap& lCompoundMethods, QObject *rParent)
                                : NMessage(lConnectionName, lMessageID, rParent), mCompoundMethodName(lCompoundMethodName), mCompoundMethodDescription(lCompoundMethodDescription), mCompoundMethods(lCompoundMethods) {
  }

  QVariantMap NMessageRegisterCompoundMethod::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lCompoundMethodNameFieldName, fCompoundMethodName());
    lMessageData.insert(lCompoundMethodDescriptionFieldName, fCompoundMethodDescription());
    lMessageData.insert(lCompoundMethodsFieldName, fCompoundMethods());
    return lMessageData;
  }

  bool NMessageRegisterCompoundMethod::fAddMethod(const QString& lMethodName) {
    if(mCompoundMethods.contains(lMethodName))
      return false;
    mCompoundMethods.insert(lMethodName, QVariantMap());
    return true;
  }

  bool NMessageRegisterCompoundMethod::fAddParameter(const QString& lMethodName, const QString &lParameterName, const QVariant &lParameterValue) {
    if(!mCompoundMethods.contains(lMethodName))
      return false;
    mCompoundMethods[lMethodName].toMap()[lParameterName] = lParameterValue;
    return true;
  }
bool NMessageRegisterCompoundMethod::fValidateMessageObject(const QJsonObject &lMessageObject) {
  if (!NMessage::fValidateMessageObject(lMessageObject)) {
    return false;
  }

  if (lMessageObject.value(cMessageTypeFieldName).toString() != cTypeRegisterCompoundMethod) {
    qDebug("Message type is not 'RegisterCompoundMethod'!");
    return false;
  }

  QJsonValue dataFields = lMessageObject.value("cFieldName_MessageData");

  return true;
}
}
