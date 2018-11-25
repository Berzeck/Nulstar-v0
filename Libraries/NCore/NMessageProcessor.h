#ifndef NMESSAGEPROCESSOR_H
#define NMESSAGEROCESSOR_H

#include <QMap>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QVariant>
#include <NMessage.h>
#include <NResponse.h>

#include "Core.h"

namespace NulstarNS {
  class CORESHARED_EXPORT NMessageProcessor : public QObject  {
    Q_OBJECT

    public:
      explicit NMessageProcessor(QObject* rParent = nullptr);
      virtual ~NMessageProcessor() {}

    public Q_SLOTS:
      void fQueueMessage(NMessage* lMessage);
      void fChangeMessagestatus(NMessage& lMessage, const NMessage::EMessageStatus& lMessageStatus);

    private:
      QMultiMap<NMessage::EMessageStatus, NMessage* > mMessageQueue; // Message Status, Message
      QMap<quint64, NMessage* > mMessageResponses; // MessageID, Message Pointer

    Q_SIGNALS:
      void sMessageStatusChanged(NMessage* lMessage, NMessage::EMessageStatus eMessageStatus);
  };
}

#endif // NPACKETBUILDER_H
