#include <QDateTime>
#include "NRequest.h"

namespace NulstarNS {
  const QString lApiRequestField("Request");
  const QString lApiRequestIDField("RequestID");
  const QString lApiRequestDateField("RequestDate");
  const QString lApiRequestTimeField("RequestTime");
  NRequest::NRequest(const QDate &lRequestDate, const QTime &lRequestTime, const QVariantMap &lRequest)
          : mStatus(ERequestStatus::eRequestNew), mID(0), mRequestDate(lRequestDate), mRequestTime(lRequestTime) {
    QVariantMap lRequestApiMap = lRequest;

    mID = QDateTime::currentMSecsSinceEpoch();
    lRequestApiMap[lApiRequestIDField] = QString::number(mID);
    lRequestApiMap[lApiRequestDateField] = lRequestDate.toString("yyyy-MM-dd");
    lRequestApiMap[lApiRequestTimeField] = lRequestTime.toString("hh:mm:ss");
    mRequest[lApiRequestField] = lRequestApiMap;
  }
}
