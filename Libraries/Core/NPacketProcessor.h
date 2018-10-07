#ifndef NPACKETPROCESSOR_H
#define NPACKETPROCESSOR_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>
#include <NRequest.h>
#include <NResponse.h>

#include "Core.h"

namespace NulstarNS {
  class CORESHARED_EXPORT NPacketProcessor : public QObject  {
    Q_OBJECT

    public:
      explicit NPacketProcessor(QObject* rParent = nullptr);

    public Q_SLOTS:
      void fProcessRequest(NRequest &lRequest);
      void fProcessResponse(NResponse& lResponse);
      
    private:
      QMap<qint64, NRequest> mPendingRequests;

    Q_SIGNALS:
      void sRequestProcessed(NRequest& lRequest);
      void sResponseProcessed(NResponse& lResponse);
  };
}

#endif // NPACKETBUILDER_H
