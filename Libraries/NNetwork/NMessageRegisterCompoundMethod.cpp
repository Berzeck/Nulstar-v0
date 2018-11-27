#include "NMessageRegisterCompoundMethod.h"

namespace NulstarNS {
    const QString lCompoundMethodNameFieldName("CompoundMethodName");
    const QString lCompoundMethodDescriptionFieldName("CompoundMethodDescription");
    const QString lCompoundMethodsFieldName("CompoundMethods");

  NMessageRegisterCompoundMethod::NMessageRegisterCompoundMethod(const QString& lConnectionName, const QString& lMessageID, const QString& lCompoundMethodName, const QString& lCompoundMethodDescription, const QVariantMap& lCompoundMethods, QObject *rParent)
                                     : NMessage(lConnectionName, lMessageID, rParent), mCompoundMethodName(lCompoundMethodName), mCompoundMethodDescription(lCompoundMethodDescription), mCompoundMethods(lCompoundMethods)  {


  }

  QVariantMap NMessageRegisterCompoundMethod::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lCompoundMethodNameFieldName, mCompoundMethodName);
    lMessageData.insert(lCompoundMethodDescriptionFieldName, mCompoundMethodDescription);
    lMessageData.insert(lCompoundMethodsFieldName, mCompoundMethods);
    return lMessageData;
  }
}
