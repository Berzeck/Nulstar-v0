#ifndef NLOGGER_H
#define NLOGGER_H

#include <QFile>
#include <QObject>
#include <QString>

#include "NTools.h"

namespace NulstarNS {
  enum class ELogLevel {eLogCritical = 1, eLogImportant = 2, eLogWarning = 3, eLogInfo = 4, eLogEverything = 5};
  enum class ELogMessageType { eMessageReceived = 1, eMessageSent = 2, eMemoryTransaction = 3, eResourceManagement = 4 };
  class NTOOLSSHARED_EXPORT NLogger : public QObject {
    Q_OBJECT

    public:
      explicit NLogger(const QString& lModuleName, const QString& lFilePath, const QString& lSeparator, ELogLevel eLogLevel, QObject* rParent = nullptr);

    public Q_SLOTS:
      void fLog(ELogLevel eLogLevel, ELogMessageType eLogMessageType, const QString& lMessage);

    private:
      QFile mLogFile;
      QString mModuleName;
      QString mFilePath;
      QString mSeparator;
      ELogLevel mLogLevel;
  };
}

#endif // NLOGGER_H
