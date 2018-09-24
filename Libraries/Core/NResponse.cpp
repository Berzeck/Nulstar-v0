#include "NResponse.h"

namespace NulstarNS {
  NResponse::NResponse(bool lSuccess, const QVariant& lReturnValue, const QString& lDescription)
           : mSuccess(lSuccess), mDescription(lDescription), mReturnValue(lReturnValue) {

     mProcessTime = QTime::currentTime();
     mProcessDate = QDate::currentDate();
  }
}
