#include "NResponse.h"

namespace NulstarNS {
  const QString lApiResponseField("Response");
  NResponse::NResponse(bool lSuccess, const QVariant& lReturnValue, const QString &lRequestDate, const QString &lRequestTime, const QString& lDescription)
           : mSuccess(lSuccess), mRequestDate(lRequestDate), mRequestTime(lRequestTime), mDescription(lDescription), mReturnValue(lReturnValue) {

     mProcessTime = QTime::currentTime();
     mProcessDate = QDate::currentDate();
  }
}
