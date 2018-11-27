#include "NUnsubscribe.h"

namespace NulstarNS {
const QString lUnsubscribeMethodsFieldName("UnsubscribeMethods");

NUnsubscribe::NUnsubscribe(const QString &lConnectionName,
                           const QString &lMessageID,
                           const QStringList &lUnscribeMethods,
                           QObject *rParent)
    : NMessage(lConnectionName, lMessageID, rParent),
      mUnscribeMethods(lUnscribeMethods) {}

QVariantMap NUnsubscribe::fMessageData() const {
  QVariantMap lMessageData;
  lMessageData.insert(lUnsubscribeMethodsFieldName, fUnsubscribeMethods());
  return lMessageData;
}

} // namespace NulstarNS
