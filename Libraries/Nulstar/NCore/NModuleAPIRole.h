#ifndef NMODULEAPIROLE_H
#define NMODULEAPIROLE_H

#include <QString>
#include <QStringList>
#include "Core.h"

namespace NulstarNS {
  class CORESHARED_EXPORT NModuleAPIRole {
    public:
      NModuleAPIRole(const QString& lRoleName, const QStringList& lVersionsSupported);
      NModuleAPIRole() { }
      NModuleAPIRole(const NModuleAPIRole& lApiRole) { mRoleName = lApiRole.mRoleName; mVersionsSupported = lApiRole.mVersionsSupported; }

      QString fRoleName() const { return mRoleName; }
      QStringList fVersionsSupported() const { return mVersionsSupported; }
      void fSetRoleName(const QString& lRoleName) { mRoleName = lRoleName; }
      void fSetVersionsSupported(const QStringList& lVersionsSupported) { mVersionsSupported = lVersionsSupported; }
      bool operator==(const NModuleAPIRole& lTargetModuleAPIRole);
      void operator=(const NModuleAPIRole& lTargetModuleAPIRole) { mRoleName = lTargetModuleAPIRole.mRoleName; mVersionsSupported = lTargetModuleAPIRole.mVersionsSupported; }

    private:
      QString mRoleName;
      QStringList mVersionsSupported;
  };
}

#endif // NMODULEAPIROLE_H
