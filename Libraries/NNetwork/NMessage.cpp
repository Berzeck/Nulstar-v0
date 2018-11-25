#include <QDateTime>
#include <QJsonDocument>

#include "NMessage.h"

namespace NulstarNS {
  const QString lMessageIDFieldName("MessageID");
  const QString lTimeStampFieldName("Timestamp");
  const QString lTimeZoneFieldName("TimeZone");
  const QString lMessageTypeFieldName("MessageType");
  const QString lMessageDataFieldName("MessageDate");

  qint64 NMessage::mLastMessageID = 0;

  NMessage::NMessage(const QString& lConnectionName, const QString& lMessageID, QObject *rParent)
          : QObject(rParent), mConnectionName(lConnectionName), mMessageID(lMessageID) {
    mTimeStamp = QDateTime::currentDateTime().currentMSecsSinceEpoch();
    mTimeZone = QDateTime::currentDateTime().offsetFromUtc() / 3600;

    if(mMessageID.isEmpty()) {
      if(mTimeStamp != mLastMessageID) mMessageID = QString::number(mTimeStamp);
      else {
        qint64 lTempID = mTimeStamp;
        while(lTempID <= mLastMessageID)
          ++lTempID;
        mMessageID = QString::number(lTempID);
      }
    }
  }

  QString NMessage::fToJsonString(QJsonDocument::JsonFormat lFormat) const {
    QVariantMap lMessage;
    lMessage.insert(lMessageIDFieldName, mMessageID);
    lMessage.insert(lTimeStampFieldName, mTimeStamp);
    lMessage.insert(lTimeZoneFieldName, mTimeZone);
    lMessage.insert(lMessageTypeFieldName, fMessageType());
    lMessage.insert(lMessageDataFieldName, fMessageData());
    return QString(QJsonDocument::fromVariant(lMessage).toJson(lFormat));
  }
}
