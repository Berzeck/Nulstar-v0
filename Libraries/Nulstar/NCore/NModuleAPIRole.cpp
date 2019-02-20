#include "NModuleAPIRole.h"

namespace NulstarNS {
  NModuleAPIRole::NModuleAPIRole(const QString& lRoleName, const QStringList &lVersionsSupported)
                : mRoleName(lRoleName), mVersionsSupported(lVersionsSupported) {

  }

  bool NModuleAPIRole::operator==(const NModuleAPIRole& lTargetModuleAPIRole) {
    if((mRoleName == lTargetModuleAPIRole.mRoleName) && (mVersionsSupported == lTargetModuleAPIRole.mVersionsSupported))
      return true;
    return false;
  }
}
