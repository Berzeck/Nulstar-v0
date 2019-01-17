#include <QJsonDocument>
#include <QDebug>

#include "NMessage.h"
#include "NMessageNegotiateConnection.h"
#include "NMessageFactory.h"

namespace NulstarNS {
  NMessageFactory::NMessageFactory(QObject* rParent)
                 : QObject(rParent) {

  }

  QJsonObject NMessageFactory::fMessageObjectFromString(const QString& lMessage, QString* rReturnMessageType) {
    QJsonDocument lJsonDoc = QJsonDocument::fromJson(lMessage.toUtf8());
    if(!lJsonDoc.isNull()) {
      QJsonObject lJsonMessage = lJsonDoc.object();
      if(lJsonMessage.contains(cMessageTypeFieldName)) {
        QString lMessageType = lJsonMessage.value(cMessageTypeFieldName).toString();
        if(rReturnMessageType)
          *rReturnMessageType = lMessageType;
        return lJsonMessage;
      }
      else {
        qDebug() << "Message received without 'Type' field!";
        return QJsonObject();
      }
    }
    qDebug() << "Text Message is not in JSon format:" << lMessage;
    return QJsonObject();
  }
}
