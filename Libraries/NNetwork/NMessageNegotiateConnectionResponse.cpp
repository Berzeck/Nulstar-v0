#include "NMessageNegotiateConnectionResponse.h"

namespace NulstarNS {
  const QString lNegotiationStatusFieldName("NegotiationStatus");
  const QString lNegotiationCommentFieldName("NegotiationComment");

  NMessageNegotiateConnectionResponse::NMessageNegotiateConnectionResponse(const QString& lConnectionName, const QString& lMessageID, QObject *rParent, quint8 lNegotiationStatus, const QString& lNegotiationComment)
                             : NMessage(lConnectionName, lMessageID, rParent), mNegotiationStatus(lNegotiationStatus), mNegotiationComment(lNegotiationComment) {


  }

  QVariantMap NMessageNegotiateConnectionResponse::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lNegotiationStatusFieldName, QString::number(mNegotiationStatus));
    lMessageData.insert(lNegotiationCommentFieldName, mNegotiationComment);
    return lMessageData;
  }


}
