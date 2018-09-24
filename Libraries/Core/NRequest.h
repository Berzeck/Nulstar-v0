#ifndef NREQUEST_H
#define NREQUEST_H

#include <QDate>
#include <QTime>
#include <QString>
#include <QVariant>

namespace NulstarNS {
  class NRequest {
    public:
      NRequest(quint64 lID, QDate lRequestDate, QTime lRequestTime, QVariant lRequest);
      quint64 fID() { return mID; }
      QDate fRequestDate() { return mRequestDate; }
      QTime fRequestTime() { return mRequestTime; }
      QVariant fReturnValue() { return mRequest; }

    private:
      quint64 mID;
      QDate mRequestDate;
      QTime mRequestTime;
      QVariant mRequest;
  };
}

#endif // NREQUEST_H
