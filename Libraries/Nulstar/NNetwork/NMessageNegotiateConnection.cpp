#include "NMessageNegotiateConnection.h"

namespace NulstarNS {
  const int cCompressionRateMax = 9;
  const int cCompressionRateMin = 0;

  NMessageNegotiateConnection::NMessageNegotiateConnection(const QString& lConnectionName, const QString& lMessageID, const QString &lProtocolVersion, const quint8 lCompressionRate, const ECompressionAlgorithm lCompressionAlgorithm, QObject *rParent)
                             : NMessage(lConnectionName, lMessageID, rParent), mCompressionAlgorithm(lCompressionAlgorithm), mProtocolVersion(lProtocolVersion) {
    if(lCompressionRate > 9)
      mCompressionRate = 9;
    else
      mCompressionRate = lCompressionRate;
  }

  QVariantMap NMessageNegotiateConnection::fMessageData() const {
    QVariantMap lMessageData;
    lMessageData.insert(cProtocolVersionFieldName, mProtocolVersion);
    lMessageData.insert(cCompressionAlgorithmFieldName, fCompressionAlgorithmString());
    lMessageData.insert(cCompressionRateFieldName, QString::number(mCompressionRate));
    return lMessageData;
  }

  QString NMessageNegotiateConnection::fCompressionAlgorithmString() const {
    QString lAlgorithm;
    if(mCompressionAlgorithm == ECompressionAlgorithm::eZlib) {
      lAlgorithm = QStringLiteral("zlib");
    }
    return lAlgorithm;
  }

  bool NMessageNegotiateConnection::fValidateMessageObject(const QJsonObject& lMessageObject) {
    if(!NMessage::fValidateMessageObject(lMessageObject)) {
      return false;
    }

    QJsonObject lDataObject = lMessageObject.value(cFieldName_MessageData).toObject();
    if(!lDataObject.contains(cProtocolVersionFieldName)) {
      qDebug("Message received without 'ProtocolVersion' field!");
      return false;
    }
    if(!lDataObject.contains(cCompressionRateFieldName)) {
      qDebug("Message received without 'CompressionRate' field!");
      return false;
    }
    int lCompressionRate = lMessageObject.value(cCompressionRateFieldName).toInt();
    if((lCompressionRate > cCompressionRateMax) || (lCompressionRate < cCompressionRateMin)) {
      qDebug("lCompressionRate value out of scope!");
      return false;
    }
    if(!lDataObject.contains(cCompressionAlgorithmFieldName)) {
      qDebug("Message received without 'CompressionAlgorithm' field!");
      return false;
    }
    return true;
  }
}
