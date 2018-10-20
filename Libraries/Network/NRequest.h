#ifndef NREQUEST_H
#define NREQUEST_H

#include <QDate>
#include <QJsonDocument>
#include <QVariantMap>
#include <QTime>
#include <QString>
#include <QVariant>

namespace NulstarNS { 
  class NRequest {
    public:
      enum class ERequestStatus {eRequestNew = 1, eRequestSent = 2, eRequestAkwdReceived = 3, eRequestProcessed = 4, eRequestSentError = 5};

      NRequest(const QDate& lRequestDate, const QTime& lRequestTime, const QVariantMap& lRequest);
      qint64 fID() const { return mID; }
      QDate fRequestDate() const { return mRequestDate; }
      QTime fRequestTime() const { return mRequestTime; }
      QVariant fRequest() const { return mRequest; }
      QVariantMap& fAdd(const QVariantMap& lRequest) { return mRequest.unite(lRequest); }
      void fSetExternalID(QString lExternalID) { mExternalID = lExternalID; }
      void fSetStatus(ERequestStatus lStatus) { mStatus = lStatus; }
      QJsonDocument fToJson() const { return QJsonDocument::fromVariant(mRequest); }
      QString fToJsonString(QJsonDocument::JsonFormat lFormat = QJsonDocument::Compact) const { return QString(QJsonDocument::fromVariant(mRequest).toJson(lFormat)); }

    private:
      ERequestStatus mStatus;
      qint64 mID;
      QString mExternalID;
      QDate mRequestDate;
      QTime mRequestTime;
      QVariantMap mRequest;
  };
}

#endif // NREQUEST_H
