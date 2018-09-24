#ifndef NRESPONSE_H
#define NRESPONSE_H

#include <QDate>
#include <QTime>
#include <QString>
#include <QVariant>

namespace NulstarNS {
  class NResponse {
    public:
      NResponse(bool lSuccess, const QVariant& lReturnValue, const QString& lDescription = QString());
      bool fSuccess() { return mSuccess; }
      quint64 fID() { return mID; }
      QString lExternalID() { return mExternalID; }
      QDate fProcessDate() { return mProcessDate; }
      QTime fProcesstime() { return mProcessTime; }
      QVariant fReturnValue() { return mReturnValue; }

      void fSetID(quint64 lID) { mID = lID; }
      void fSetExternalID(QString lExternalID) { mExternalID = lExternalID; }

    private:
      bool mSuccess;
      quint64 mID;
      QString mExternalID;
      QDate mProcessDate;
      QTime mProcessTime;
      QString mDescription;
      QVariant mReturnValue;
  };
}

#endif // NRESPONSE_H
