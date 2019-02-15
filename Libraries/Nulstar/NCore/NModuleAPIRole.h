#ifndef NMODULEAPIROLE_H
#define NMODULEAPIROLE_H

#include <QString>
#include <QVersionNumber>
#include "Core.h"

namespace NulstarNS {
  class CORESHARED_EXPORT NModuleAPIRole {
    public:
      NModuleAPIRole(const QString& lRoleName, const QVersionNumber& lVersionNumber);
      NModuleAPIRole() { }

      QString fRoleName() const { return mRoleName; }
      QVersionNumber fVersionNumber() const { return mVersionNumber; }
      void fSetRoleName(const QString& lRoleName) { mRoleName = lRoleName; }
      void fSetVersionNumber(const QVersionNumber& lVersionNumber) { mVersionNumber = lVersionNumber; }
      bool operator==(const NModuleAPIRole& lTargetModuleAPIRole) { if((mRoleName == lTargetModuleAPIRole.fRoleName()) && (mVersionNumber == lTargetModuleAPIRole.fVersionNumber())) return true; else return false; }

    private:
      QString mRoleName;
      QVersionNumber mVersionNumber;
  };
}

#endif // NMODULEAPIROLE_H
