#include "NConnectionInfo.h"

namespace NulstarNS {
  NConnectionInfo::NConnectionInfo(const quint8 lReconnectionTryInterval, const QString& lRole, const QString& lIP, const QString& lPort, const QString& lAPIVersion)
                 : mReconnectionTryInterval(lReconnectionTryInterval), mRole(lRole), mIP(lIP), mPort(lPort), mAPIVersion(lAPIVersion) {

  }

  bool NConnectionInfo::fIsValid() {
    if( mRole.isEmpty() || mIP.isEmpty() || mPort.isEmpty() || mAPIVersion.isEmpty())
      return false;
    return true;
  }
}
