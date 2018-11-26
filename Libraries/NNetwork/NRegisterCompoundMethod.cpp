#include "NRegisterCompoundMethod.h"

namespace NulstarNS {
const QString lCompoundMethodNameFieldName("CompoundMethodName");
const QString lCompoundMethodDescriptionFieldName("CompoundMethodDescription");
const QString lCompoundMethodsFieldName("CompoundMethods");
NRegisterCompoundMethod::NRegisterCompoundMethod(const QString& lConnectionName, const QString& lMessageID, const QString& lCompoundMethodName, const QString& lCompoundMethodDescription, const QVariantMap& lCompoundMethods, QObject* rParent)
    : NMessage(lConnectionName, lMessageID, rParent)
    , mCompoundMethodName(lCompoundMethodName)
    , mCompoundMethodDescription(lCompoundMethodDescription)
    , mCompoundMethods(lCompoundMethods)
{
}

QVariantMap NRegisterCompoundMethod::fMessageData() const
{
    QVariantMap lMessageData;
    lMessageData.insert(lCompoundMethodNameFieldName, fCompoundMethodName());
    lMessageData.insert(lCompoundMethodDescriptionFieldName, fCompoundMethodDescription());
    lMessageData.insert(lCompoundMethodsFieldName, fCompoundMethods());
    return lMessageData;
}

}
