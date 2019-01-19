#ifndef NMESSAGERESPONSE_H
#define NMESSAGERESPONSE_H

#include <QObject>
#include <QString>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageResponse : public NMessage {
    Q_OBJECT

  public:
    enum class EResponseStatus { eResponseError = 0, eResponseSuccessful = 1 };

    explicit NMessageResponse(const QString& lConnectionName, const QString& lMessageID = QString(), const QString& lRequestID = QString(), const quint64 lResponseProcessingTime = 0, const EResponseStatus lResponseStatus = EResponseStatus::eResponseSuccessful,
                              const QString& lResponseComment = QString(), const quint64 lResponseMaxSize = 0, const QVariantMap& lResponseData = QVariantMap(), QObject* rParent = nullptr);
    ~NMessageResponse() override {}

    QString fRequestID() const { return mRequestID; }
    quint64 fResponseProcessingTime() const { return mResponseProcessingTime; }
    EResponseStatus fResponseStatus() const { return mResponseStatus; }
    QString fResponseComment() const { return mResponseComment; }
    quint64 fResponseMaxSize() const { return mResponseMaxSize; }
    QVariantMap fResponseData() const { return mResponseData; }

    bool fAddMethod(const QString& lMethodName);
    bool fAddParameter(const QString& lMethodName, const QString& lParameterName, const QVariant& lParameterValue);
    static bool fValidateMessageObject(const QJsonObject& lMessageObject);

  protected:
    QVariantMap fMessageData() const override;
    QString fMessageType() const override { return cTypeReponse; }

  private:
    QString mRequestID;
    quint64 mResponseProcessingTime;
    EResponseStatus mResponseStatus;
    QString mResponseComment;
    quint64 mResponseMaxSize;
    QVariantMap mResponseData;
  };
} // namespace NulstarNS

#endif // NMESSAGERESPONSE_H
