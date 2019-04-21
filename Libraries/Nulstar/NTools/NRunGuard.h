#ifndef NRUNGUARD_H
#define NRUNGUARD_H

#include <QSharedMemory>
#include <QString>
#include <QSystemSemaphore>

#include "NTools.h"

namespace NulstarNS {
  class NTOOLSSHARED_EXPORT NRunGuard{
    public:
      NRunGuard(const QString& lKey);
      ~NRunGuard();

      bool fIsAnotherRunning();
      bool fTryToRun();
      QString fGenerateKeyHash(const QString& lKey, const QString& lSalt);
      void fRelease();

    private:
      const QString mKey;
      const QString mMemLockKey;
      const QString mSharedmemKey;

      QSharedMemory mSharedMem;
      QSystemSemaphore mMemLock;
  };
}

#endif // NRUNGUARD_H
