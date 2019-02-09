#ifndef NMODULEAPI_H
#define NMODULEAPI_H

#include <QHostAddress>
#include <QString>
#include <QVariantMap>
#include "Core.h"

namespace NulstarNS {
  class CORESHARED_EXPORT NModuleAPI : public QVariantMap {
    public:
      NModuleAPI(const QVariantMap& lModuleApiMap);

      bool fIsValid() { return mIsValid; }

    protected:
      void fFillFields(const QVariantMap& lModuleApiMap);

    private:
      bool mIsValid;
      QVariantMap lDependencies;
      QHostAddress lIP;
      QVariantMap lMethods;
      QString lModuleAbbreviation;
      QString lModuleDomain;
      QString lModuleName;
      QVariantMap lModuleRoles;
      QString lModuleVersion;
      quint16 lPort;
  };
}

#endif // NMODULEAPI_H
