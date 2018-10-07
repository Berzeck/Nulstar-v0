#include <QDateTime>
#include "NPacketProcessor.h"

namespace NulstarNS {
  NPacketProcessor::NPacketProcessor(QObject* rParent)
                  : QObject(rParent) {

  }

  void NPacketProcessor::fProcessRequest(NRequest& lRequest) {
    while(mPendingRequests.contains(QDateTime::currentMSecsSinceEpoch()))
      continue;
    qint64 lMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch());
    lRequest.fSetID(lMSecsSinceEpoch);

    mPendingRequests.insert(lMSecsSinceEpoch, lRequest);
    emit sRequestProcessed(lRequest);
  }

  void NPacketProcessor::fProcessResponse(NResponse &lResponse) {

  }
}
