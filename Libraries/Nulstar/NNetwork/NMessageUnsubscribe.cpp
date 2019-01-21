#include "NMessageUnsubscribe.h"

namespace NulstarNS {
  const QString lUnsubscribeMethodsFieldName("UnsubscribeMethods");

  NMessageUnsubscribe::NMessageUnsubscribe(const QString &lConnectionName, const QString &lMessageID, const QStringList &lUnscribeMethods, QObject *rParent)
                     : NMessage(lConnectionName, lMessageID, rParent), mUnsubscribeMethods(lUnscribeMethods) {

 }

  QVariantMap NMessageUnsubscribe::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lUnsubscribeMethodsFieldName, mUnsubscribeMethods);
    return lMessageData;
  }
bool NMessageUnsubscribe::fValidateMessageObject(const QJsonObject &lMessageObject) {
  if (!NMessage::fValidateMessageObject(lMessageObject)) {
    return false;
  }

  if (lMessageObject.value(cMessageTypeFieldName).toString() != cTypeUnsubscribe) {
    qDebug("Message type is not 'Unsubscribe'!");
    return false;
  }


  return true;
}
} // namespace NulstarNS
