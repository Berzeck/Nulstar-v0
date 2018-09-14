#include "NResponse.h"

namespace NulstarNS {
  NResponse::NResponse(quint64 lID, QString lExternalID, bool lSuccess, const QVariant& lReturnValue, const QString& lDescription)
           : mSuccess(lSuccess), mID(lID), mExternalID(lExternalID), mDescription(lDescription), mReturnValue(lReturnValue) {

     mProcessTime = QTime::currentTime();
     mProcessDate = QDate::currentDate();
  }
}
