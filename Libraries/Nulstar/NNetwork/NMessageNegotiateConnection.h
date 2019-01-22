#ifndef NMESSAGENEGOTIATECONNECTION_H
#define NMESSAGENEGOTIATECONNECTION_H

#include <QObject>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  const QString cProtocolVersionFieldName("ProtocolVersion");
  const QString cCompressionRateFieldName("CompressionRate");
  const QString cCompressionAlgorithmFieldName("CompressionAlgorithm");
  class NETWORKSHARED_EXPORT NMessageNegotiateConnection : public NMessage {
    Q_OBJECT

    public:
      enum class ECompressionAlgorithm { eZlib = 1 };

      explicit NMessageNegotiateConnection(const QString& lConnectionName, const QString& lMessageID = QString(), const QString& lProtocolVersion = QString(), const quint8 lCompressionRate = 0,
                                           const ECompressionAlgorithm lCompressionAlgorithm = ECompressionAlgorithm::eZlib, QObject* rParent = nullptr);
      ~NMessageNegotiateConnection() override {}

      ECompressionAlgorithm fCompressionAlgorithm() const { return mCompressionAlgorithm; }
      quint8 fCompressionRate() const { return mCompressionRate; }      
      static bool fValidateMessageObject(const QJsonObject& lMessageObject);

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return QString(cTypeNegotiateConnection); }

    private:
      quint8 mCompressionRate;
      ECompressionAlgorithm mCompressionAlgorithm;
      QString mProtocolVersion;
      QString fCompressionAlgorithmString() const;            
  };
}

#endif // NMESSAGENEGOTIATECONNECTION_H
