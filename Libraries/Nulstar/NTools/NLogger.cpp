#include "NLogger.h"

namespace NulstarNS {
  NLogger::NLogger(const QString& lModuleName, const QString& lFilePath, const QString& lSeparator, QObject* rParent)
         : QObject(rParent) {
    Q_UNUSED(lModuleName)
    Q_UNUSED(lFilePath)
    Q_UNUSED(lSeparator)
  }
}
