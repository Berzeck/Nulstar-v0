#ifndef NMESSAGERESPONSE_H
#define NMESSAGERESPONSE_H

#include <QObject>
#include <QString>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  const QString cRequestIDFieldName("RequestID");
  const QString cResponseProcessingTimeFieldName("ResponseProcessingTime");
  const QString cResponseStatusFieldName("ResponseStatus");
  const QString cResponseCommentFieldName("ResponseComment");
  const QString cResponseMaxSizeFieldName("ResponseMaxSize");
  const QString cResponseDataFieldName("ResponseData");
  const QString cResponseDataDependencies("Dependencies");
  const QString cResponseErrorCodeFieldName("ResponseErrorCode");
  const int cResponseStatusSuccess = 1;
  const int cResponseStatusFailure = 0;

  class NETWORKSHARED_EXPORT NMessageResponse : public NMessage {
    Q_OBJECT

    public:
      enum class EResponseStatus { eResponseSuccessful = 0,
                                   /* for NULStar modules*/
                                   eResponseMethodExeError = 1, eResponseNoMethodError = 2,
                                   eResponseDependenciesNoFoundError = 3, eResponseMethodExistedError = 4,
                                   eResponseMethodUnavailableError = 5, eResponseMethodListEmptyError = 6,
                                   eResponseMessageFormatError = 7,


                                   /*for NULS 2.0 modules*/
                                   eResponseHandleException = 65536,
                                 };

      explicit NMessageResponse(const QString& lConnectionName, const QString& lMessageID = QString(), const QString& lRequestID = QString(), const qint64 lResponseProcessingTime = 0, const EResponseStatus lResponseStatus = EResponseStatus::eResponseSuccessful,
                                const QString& lResponseComment = QString(), const quint64 lResponseMaxSize = 0, const QVariantMap& lResponseData = QVariantMap(), const QString& lResponseErrorCode = QString(), QObject* rParent = nullptr);
      ~NMessageResponse() override {}

      QString fRequestID() const { return mRequestID; }
      qint64 fResponseProcessingTime() const { return mResponseProcessingTime; }
      EResponseStatus fResponseStatus() const { return mResponseStatus; }
      QString fResponseComment() const { return mResponseComment; }
      quint64 fResponseMaxSize() const { return mResponseMaxSize; }
      QVariantMap fResponseData() const { return mResponseData; }

      bool fAddMethod(const QString& lMethodName);
      bool fAddParameter(const QString& lMethodName, const QString& lParameterName, const QVariant& lParameterValue);
      static bool fValidateMessageObject(const QJsonObject& lMessageObject);
      static qint64 fCalculateResponseProccessingTime(qint64 lStartingTimestamp);

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return cTypeReponse; }

    private:
      QString mRequestID;
      qint64 mResponseProcessingTime;
      EResponseStatus mResponseStatus;
      QString mResponseComment;
      quint64 mResponseMaxSize;
      QVariantMap mResponseData;
      QString mResponseErrorCode;
  };
} // namespace NulstarNS

#endif // NMESSAGERESPONSE_H
