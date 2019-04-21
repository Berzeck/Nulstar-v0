#include <QByteArray>
#include <QCryptographicHash>

#include "NRunGuard.h"

namespace NulstarNS {
  const QString cMemLockKeySalt(QStringLiteral("__NulstarMemLockKeySalt__"));
  const QString cSharedMemKeySalt(QStringLiteral("__NulstarSharedMemKeySalt__"));

  NRunGuard::NRunGuard(const QString& lKey)
           : mKey(lKey), mMemLockKey(fGenerateKeyHash(lKey, cMemLockKeySalt)), mSharedmemKey(fGenerateKeyHash(lKey, cSharedMemKeySalt)), mSharedMem(mSharedmemKey), mMemLock(mMemLockKey, 1) {
    QSharedMemory lFix(mSharedmemKey);
    lFix.attach();
  }

  NRunGuard::~NRunGuard() {
    fRelease();
  }

  QString NRunGuard::fGenerateKeyHash(const QString& lKey, const QString& lSalt) {
    QByteArray lData;
    lData.append(lKey.toUtf8());
    lData.append(lSalt.toUtf8());
    lData = QCryptographicHash::hash(lData, QCryptographicHash::Sha1).toHex();
    return lData;
  }


  bool NRunGuard::fIsAnotherRunning() {
    if(mSharedMem.isAttached())
      return false;
    mMemLock.acquire();
    const bool lIsRunning = mSharedMem.attach();
    if(lIsRunning)
      mSharedMem.detach();
    mMemLock.release();
    return lIsRunning;
  }

  bool NRunGuard::fTryToRun() {
    if(fIsAnotherRunning())
      return false;
    mMemLock.acquire();
    const bool lResult = mSharedMem.create(sizeof(quint64));
    mMemLock.release();
    if(!lResult ) {
      fRelease();
      return false;
    }
    return true;
  }

  void NRunGuard::fRelease() {
    mMemLock.acquire();
    if(mSharedMem.isAttached())
      mSharedMem.detach();
    mMemLock.release();
  }
}
