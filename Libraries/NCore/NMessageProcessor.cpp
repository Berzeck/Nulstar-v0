#include "NMessageProcessor.h"

namespace NulstarNS {
  const quint8 lSecondsToRetryMessageDelivery = 10;

  NMessageProcessor::NMessageProcessor(QObject* rParent)
                  : QObject(rParent) {

  }

  void NMessageProcessor::fChangeMessagestatus(NMessage &lMessage, const NMessage::EMessageStatus& lMessageStatus) {
    lMessage.fSetStatus(lMessageStatus);
  }

  void NMessageProcessor::fQueueMessage(NMessage* lMessage) {
    mMessageQueue.insertMulti(lMessage->fConnectionName(), lMessage);
    mMessageResponses.insert(lMessage->fMessageID().toULongLong(), lMessage);
    emit sMessageStatusChanged(lMessage, NMessage::EMessageStatus::eAwaitingDelivery);
  }
}
