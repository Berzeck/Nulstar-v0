#include "NPacketProcessor.h"

namespace NulstarNS {
  NPacketProcessor::NPacketProcessor(QObject* rParent)
                  : QObject(rParent) {

  }

  void NPacketProcessor::fProcessRequest(NRequest& lRequest) {
    mPendingRequests.insert(lRequest.fID(), lRequest);
    emit sRequestProcessed(lRequest);
  }

  void NPacketProcessor::fProcessResponse(NResponse &lResponse) {

  }
}
