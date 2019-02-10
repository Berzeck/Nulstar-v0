#ifndef NMODULEAPI_H
#define NMODULEAPI_H

#include <QHostAddress>
#include <QString>
#include <QVariantMap>
#include "Core.h"

namespace NulstarNS {
  class CORESHARED_EXPORT NModuleAPI : public QVariantMap {
    public:
      NModuleAPI();
      ~NModuleAPI() { }
      NModuleAPI(const QVariantMap& lModuleApiMap);

      bool fIsValid() { return mIsValid; }
      QString fModuleName() { return mModuleName; }
      QString fMessageID() { return mMessageID; }
      QString fWebSocketID() { return mWebSocketID; }
      QString fWebSocketServerName() { return mWebSocketServerName; }
      void fSetMessageID(const QString& lMessageID) { mMessageID = lMessageID ; }
      void fSetWebSocketServerName(const QString& lWebSocketServerName) { mWebSocketServerName = lWebSocketServerName; }
      void fSetWebSocketID(const QString& lWebSocketID) { mWebSocketID = lWebSocketID; }
      bool fIsRoleSupported(const QString& lRoleName, const QString& lVersion);

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
      QVariantMap mModuleRoles;
      QString mModuleVersion;
      quint16 mPort;

      QString mMessageID;
      QString mWebSocketID;
      QString mWebSocketServerName;
  };
}

Q_DECLARE_METATYPE(NulstarNS::NModuleAPI)

#endif // NMODULEAPI_H
