#include "NRequest.h"

namespace NulstarNS {
  NRequest::NRequest(const QDate &lRequestDate, const QTime &lRequestTime, const QVariantMap &lRequest)
          : mStatus(ERequestStatus::eRequestNew), mID(0), mRequestDate(lRequestDate), mRequestTime(lRequestTime), mRequest(lRequest) {

  }
}
