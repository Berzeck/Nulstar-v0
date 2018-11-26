#include "NUnregisterCompoundMethod.h"

namespace NulstarNS {
const QString lUnregisterCompoundMethodNameFieldName("UnregisterCompoundMethodName");
NUnregisterCompoundMethod::NUnregisterCompoundMethod(const QString& lConnectionName, const QString& lMessageID, const QString& lUnregisterCompoundMethodName, QObject* rParent)
    : NMessage(lConnectionName, lMessageID, rParent)
    , mUnregisterCompoundMethodName(lUnregisterCompoundMethodName)
{
}

QVariantMap NUnregisterCompoundMethod::fMessageData() const
{
    QVariantMap lMessageData;
    lMessageData.insert(lUnregisterCompoundMethodNameFieldName, fUnregisterCompoundMethodName());
    return lMessageData;
}

}
