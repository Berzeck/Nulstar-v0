#ifndef NMODULEAPI_H
#define NMODULEAPI_H

#include <QHostAddress>
#include <QList>
#include <QString>
#include <QVariantMap>
#include "Core.h"
#include "NModuleAPIRole.h"

namespace NulstarNS {
  class CORESHARED_EXPORT NModuleAPI {
    public:
      NModuleAPI();
      NModuleAPI(const QVariantMap& lModuleApiMap);
      ~NModuleAPI();

      bool fIsValid() { return mIsValid; }
      QVariantMap fDependencies() const { return mDependencies; }
      quint8 fFindDependenciesRetryCounter() const { return mFindDependenciesRetryCounter; }
      QString fModuleName() const { return mModuleName; }
      QString fMessageID() const { return mMessageID; }
      qint64 fMSecsSinceEpoch() const { return mMSecsSinceEpoch; }
      QString fWebSocketID() const { return mWebSocketID; }
      QString fWebSocketServerName() const { return mWebSocketServerName; }
      QHostAddress fIP() const { return mIP; }
      quint16 fPort() const { return mPort; }
      QList<NModuleAPIRole> fModuleRoles() const { return mModuleRoles; }
      void fSetMessageID(const QString& lMessageID) { mMessageID = lMessageID ; }
      void fSetWebSocketServerName(const QString& lWebSocketServerName) { mWebSocketServerName = lWebSocketServerName; }
      void fSetWebSocketID(const QString& lWebSocketID) { mWebSocketID = lWebSocketID; }
      void fSetMSecsSinceEpoch(qint64 lMSecsSinceEpoch) { mMSecsSinceEpoch = lMSecsSinceEpoch; }
      void fSetFindDependenciesRetryCounter(quint8 lFindDependenciesRetryCounter) { mFindDependenciesRetryCounter = lFindDependenciesRetryCounter; }
      bool fIsRoleSupported(const NModuleAPIRole& lModuleRoleName) const;
      bool fAreRolesNeeded(const QList<NModuleAPIRole>& lModuleRoles) const;
      bool operator==(NModuleAPI const& lTargetAPI) { if((mModuleName == lTargetAPI.fModuleName()) && (mWebSocketID == lTargetAPI.fWebSocketID())) return true; return false; }
      bool operator<(NModuleAPI const& lTargetAPI) { if((mModuleName < lTargetAPI.fModuleName())) return true; return false; }

    protected:
      void fFillFields(const QVariantMap& lModuleApiMap);

    private:
      bool mIsValid;
      quint8 mFindDependenciesRetryCounter;
      QVariantMap mDependencies;
      QHostAddress mIP;
      QVariantMap mMethods;
      QString mModuleAbbreviation;
      QString mModuleDomain;
      QString mModuleName;
      QList<NModuleAPIRole> mModuleRoles;
      QString mModuleVersion;
      quint16 mPort;

      qint64 mMSecsSinceEpoch;
      QString mMessageID;
      QString mWebSocketID;
      QString mWebSocketServerName;
  };
}


#endif // NMODULEAPI_H
