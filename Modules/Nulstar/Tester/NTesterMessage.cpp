#include <QDateTime>
#include <QTimeZone>

#include "NTesterMessage.h"
#include "NMessageRequest.h"

namespace NulstarNS {


  NTesterMessage::NTesterMessage(const QString& lProtocolVersion, const QUrl& lConnectionUrl, quint8 lConnectionRetryInterval, QWebSocket* rSocket, QObject* rParent)
                : NWebSocket(cServiceManagerName, lProtocolVersion, lConnectionUrl, lConnectionRetryInterval, rSocket, rParent) {

  }
  void NTesterMessage::fConnectServiceManager() {
      fConnect();
      connect(this, &NWebSocket::sStateChanged, this, &NTesterMessage::fOnConnectionStateChanged);
  }

  void NTesterMessage::fSendBulkRequestMessage() {
      int lIndex = 0;
      int lIndexMax = 1000;
      QString lMessage("{\"MessageData\":{\"RequestAck\":false,\"RequestMethods\":{\"RegisterAPI\":{\"Abbreviation\":\"NTS\",\"ConnectionInformation\":{\"IP\":\"127.0.0.1\",\"Port\":\"7779\"},\"Dependencies\":{\"Role_ServiceManager\":\"0.1\"},\"Methods\":[],\"ModuleDomain\":\"Nulstar\",\"ModuleName\":\"Tester\",\"ModuleRoles\":{\"Role_Tester[0.1]\":[\"0.1\"]},\"ModuleVersion\":\"0.1.0\"}},\"ResponseMaxSize\":\"0\",\"SubscriptionEventCounter\":\"0\",\"SubscriptionPeriod\":\"0\",\"SubscriptionRange\":\"\"},\"MessageID\":\"1549960969557-3\",\"MessageType\":\"Request\",\"TimeZone\":8,\"Timestamp\":1549960969664}");
      //QString lMessage("{\"MessageData\":{\"CompressionAlgorithm\":\"zlib\",\"CompressionRate\":\"0\",\"ProtocolVersion\":\"0.1\"},\"MessageID\":\"1549963086154-1\",\"MessageType\":\"NegotiateConnection\",\"TimeZone\":8,\"Timestamp\":1549963086154}");
      qint64 lBeginTime = QDateTime::currentDateTime().currentMSecsSinceEpoch();
      for (; lIndex < lIndexMax; lIndex++){
        fSendTextMessage(lMessage);
      }

      qint64 lEndTime = QDateTime::currentDateTime().currentMSecsSinceEpoch();
      qDebug("%s", qUtf8Printable(QString("Sending '%1' messages costs %2 ms!").arg(lIndexMax).arg(lEndTime - lBeginTime)));
  }

  void NTesterMessage::fOnConnectionStateChanged(NWebSocket::EConnectionState lNewState) {
     NWebSocket* rWebSocket = qobject_cast<NWebSocket* > (sender());
     if(rWebSocket) {
         switch(lNewState) {
           case NWebSocket::EConnectionState::eConnectionActive: {
             fSendBulkRequestMessage();
             break;
           }
           default:
             break;
         }
     }
  }
}
