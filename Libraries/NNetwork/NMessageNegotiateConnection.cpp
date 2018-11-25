#include "NMessageNegotiateConnection.h"

namespace NulstarNS {
  const QString lCompressionRateFieldName("CompressionRate");
  const QString lCompressionAlgorithmFieldName("CompressionAlgorithm");

  NMessageNegotiateConnection::NMessageNegotiateConnection(const QString& lConnectionName, const QString& lMessageID, const quint8 lCompressionRate, const ECompressionAlgorithm lCompressionAlgorithm, QObject *rParent)
                             : NMessage(lConnectionName, lMessageID, rParent), mCompressionAlgorithm(lCompressionAlgorithm) {
    if(lCompressionRate > 9) mCompressionRate = 9;
    else mCompressionRate = lCompressionRate;
  }

  QVariantMap NMessageNegotiateConnection::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(lCompressionAlgorithmFieldName, fCompressionAlgorithmString());
    lMessageData.insert(lCompressionRateFieldName, QString::number(mCompressionRate));
    return lMessageData;
  }

  QString NMessageNegotiateConnection::fCompressionAlgorithmString() const {
    QString lAlgorithm;
    if(mCompressionAlgorithm == ECompressionAlgorithm::eZlib) {
      lAlgorithm = QStringLiteral("zlib");
    }
    return lAlgorithm;
  }
}
