#ifndef NRESPONSE_H
#define NRESPONSE_H

#include <QDate>
#include <QTime>
#include <QString>
#include <QVariant>

namespace NulstarNS {
  class NResponse {
    public:
      NResponse(bool lSuccess, const QVariant& lReturnValue, const QString& lRequestDate = QDate::currentDate().toString("yyyy-MM-dd"), const QString& lRequestTime = QTime::currentTime().toString("hh:mm:ss"), const QString& lDescription = QString());
      bool fSuccess() const { return mSuccess; }
      quint64 fID() const { return mID; }
      QString lExternalID() const { return mExternalID; }
      QDate fProcessingDate() const { return mProcessDate; }
      QTime fProcessingTime() const { return mProcessTime; }
      QString fRequestDate() const { return mRequestDate; }
      QString fRequestTime() const { return mRequestTime; }
      QVariant fReturnValue() const { return mReturnValue; }

      void fSetID(quint64 lID) { mID = lID; }
      void fSetExternalID(QString lExternalID) { mExternalID = lExternalID; }

    private:
      bool mSuccess;
      quint64 mID;
      QString mExternalID;
      QDate mProcessDate;
      QTime mProcessTime;
      QString mRequestDate;
      QString mRequestTime;
      QString mDescription;
      QVariant mReturnValue;
  };
}

#endif // NRESPONSE_H
