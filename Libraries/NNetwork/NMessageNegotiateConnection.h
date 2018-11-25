#ifndef NMESSAGENEGOTIATECONNECTION_H
#define NMESSAGENEGOTIATECONNECTION_H

#include <QObject>
#include <QVariantMap>

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageNegotiateConnection : public NMessage {
    Q_OBJECT

    public:
      enum class ECompressionAlgorithm { eZlib = 1 };

      explicit NMessageNegotiateConnection(const QString& lConnectionName, const QString& lMessageID = QString(), const quint8 lCompressionRate = 0, const ECompressionAlgorithm lCompressionAlgorithm = ECompressionAlgorithm::eZlib, QObject* rParent = nullptr);
      ~NMessageNegotiateConnection() override {}

      ECompressionAlgorithm fCompressionAlgorithm() const { return mCompressionAlgorithm; }
      quint8 fCompressionRate() const { return mCompressionRate; }      

    protected:
      QVariantMap fMessageData() const override;
      QString fMessageType() const override { return QString("NegotiateConnection"); }

    private:
      ECompressionAlgorithm mCompressionAlgorithm;
      QString fCompressionAlgorithmString() const;
      quint8 mCompressionRate;
  };
}

#endif // NMESSAGENEGOTIATECONNECTION_H
