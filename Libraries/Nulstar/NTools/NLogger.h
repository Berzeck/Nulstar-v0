#ifndef NLOGGER_H
#define NLOGGER_H

#include <QObject>
#include <QString>

#include "NTools.h"

namespace NulstarNS {
  class NLogger : public QObject {
    Q_OBJECT

    public:
      enum class ELogMessageType { eMessageReceived = 1, eMessageSent = 2 };

      explicit NTOOLSSHARED_EXPORT NLogger(const QString& lModuleName, const QString& lFilePath, const QString& lSeparator, QObject* rParent = nullptr);

    public Q_SLOTS:
     // void fLog(ELogMessageType eLogMessageType, const QString& lFunctionName, const QString& lMessage) {}
  };
}

#endif // NLOGGER_H
