#include "NRequest.h"

namespace NulstarNS {
  NRequest::NRequest(quint64 lID, QDate lRequestDate, QTime lRequestTime, QVariant lRequest)
          : mID(lID), mRequestDate(lRequestDate), mRequestTime(lRequestTime), mRequest(lRequest) {

  }
}
