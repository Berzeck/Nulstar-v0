#include "NMessageNegotiateConnectionResponse.h"

namespace NulstarNS {
  const QString lNegotiationStatusFieldName("NegotiationStatus");
  const QString lNegotiationCommentFieldName("NegotiationComment");

  NMessageNegotiateConnectionResponse::NMessageNegotiateConnectionResponse(const QString& lConnectionName, const QString& lMessageID, ENegotiationStatus lNegotiationStatus, const QString& lNegotiationComment, QObject *rParent)
                                     : NMessage(lConnectionName, lMessageID, rParent), mNegotiationStatus(lNegotiationStatus), mNegotiationComment(lNegotiationComment) {


  }

  QVariantMap NMessageNegotiateConnectionResponse::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lNegotiationStatusFieldName, mNegotiationStatus == ENegotiationStatus::eNegotiationError ? QString("0") : QString("1"));
    lMessageData.insert(lNegotiationCommentFieldName, mNegotiationComment);
    return lMessageData;
  }
}
