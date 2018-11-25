#include "NMessageProcessor.h"

namespace NulstarNS {
  const quint8 lSecondsToRetryMessageDelivery = 10;

  NMessageProcessor::NMessageProcessor(QObject* rParent)
                  : QObject(rParent) {

  }

  void NMessageProcessor::fChangeMessagestatus(NMessage &lMessage, const NMessage::EMessageStatus& lMessageStatus) {
    QMapIterator<NMessage::EMessageStatus, NMessage*> i1(mMessageQueue);
    while(i1.hasNext()) {
      i1.next();
      QString lMessageID = i1.value()->fMessageID();
      if(lMessageID == lMessage.fMessageID()) {
        mMessageQueue.remove(i1.key(), i1.value());
        mMessageQueue.insert(lMessageStatus, &lMessage);
        return;
      }
    }
  }

  void NMessageProcessor::fQueueMessage(NMessage* lMessage) {
    mMessageQueue.insertMulti(NMessage::EMessageStatus::eAwaitingDelivery, lMessage);
    mMessageResponses.insert(lMessage->fMessageID().toULongLong(), lMessage);
    emit sMessageStatusChanged(lMessage, NMessage::EMessageStatus::eAwaitingDelivery);
  }
}
