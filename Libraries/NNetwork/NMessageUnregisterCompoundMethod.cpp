#include "NMessageUnregisterCompoundMethod.h"

namespace NulstarNS {
  const QString lUnregisterCompoundMethodName("UnregisterCompoundMethodName");

  NMessageUnregisterCompoundMethod::NMessageUnregisterCompoundMethod(const QString& lConnectionName, const QString& lMessageID, const QString& lUnregisterCompoundMethodName, QObject *rParent)
                                     : NMessage(lConnectionName, lMessageID, rParent), mUnregisterCompoundMethodName(lUnregisterCompoundMethodName) {


  }

  QVariantMap NMessageUnregisterCompoundMethod::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lUnregisterCompoundMethodName, mUnregisterCompoundMethodName);

    return lMessageData;
  }
}
