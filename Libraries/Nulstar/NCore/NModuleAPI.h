#ifndef NMODULEAPI_H
#define NMODULEAPI_H

#include <QHostAddress>
#include <QList>
#include <QString>
#include <QVariantMap>
#include <QVersionNumber>
#include "Core.h"
#include "NModuleAPIRole.h"

namespace NulstarNS {
  class CORESHARED_EXPORT NModuleAPI {
    public:
      NModuleAPI();
      NModuleAPI(const QVariantMap& lModuleApiMap);

      bool fIsValid() { return mIsValid; }
      QString fModuleName() const { return mModuleName; }
      QString fMessageID() const { return mMessageID; }
      QString fWebSocketID() const { return mWebSocketID; }
      QString fWebSocketServerName() const { return mWebSocketServerName; }
      void fSetMessageID(const QString& lMessageID) { mMessageID = lMessageID ; }
      void fSetWebSocketServerName(const QString& lWebSocketServerName) { mWebSocketServerName = lWebSocketServerName; }
      void fSetWebSocketID(const QString& lWebSocketID) { mWebSocketID = lWebSocketID; }
    //  bool fDoesAPISatisfyDependence(const NModuleAPIlVersion) const;
   //   bool fIsRoleSupported(const QString& lRoleName, const QVersionNumber& lVersion) const;

    protected:
      void fFillFields(const QVariantMap& lModuleApiMap);

    private:
      bool mIsValid;
      QVariantMap mDependencies;
      QHostAddress mIP;
      QVariantMap mMethods;
      QString mModuleAbbreviation;
      QString mModuleDomain;
      QString mModuleName;
      QList<NModuleAPIRole> mModuleRoles;
      QString mModuleVersion;
      quint16 mPort;

      QString mMessageID;
      QString mWebSocketID;
      QString mWebSocketServerName;
  };
}


#endif // NMODULEAPI_H
