#include <QDateTime>
#include <QFileInfo>
#include <QString>
#include <QTextStream>
#include "NLogger.h"

namespace NulstarNS {
  const QString cDateFormat(QStringLiteral("yyMMdd"));

  NLogger::NLogger(const QString& lModuleName, const QString& lFilePath, const QString& lSeparator, ELogLevel eLogLevel, QObject* rParent)
         : QObject(rParent), mModuleName(lModuleName), mFilePath(lFilePath), mSeparator(lSeparator), mLogLevel(eLogLevel) {

    mLogFile.setFileName(QString("%1/%2-%3.log").arg(lFilePath).arg(lModuleName).arg(QDateTime::currentDateTime().date().toString("yyMMdd")));
    mLogFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
  }

  void NLogger::fLog(ELogLevel eLogLevel, ELogMessageType eLogMessageType, const QString& lMessage) {
    if(eLogLevel > mLogLevel)
      return;
    QFileInfo lFileInfo(mLogFile);
    QString lFileDate(lFileInfo.completeBaseName().split("-").at(1));
    QString lDate = QDateTime::currentDateTime().date().toString("yyMMdd");
    if(lFileDate != lDate) {
      if(mLogFile.isOpen())
        mLogFile.close();
      mLogFile.setFileName(QString("%1/%2-%3.log").arg(mFilePath).arg(mModuleName).arg(QDateTime::currentDateTime().date().toString("yyMMdd")));
      mLogFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
    QString lLogLevel;
    switch(eLogLevel) {
      case ELogLevel::eLogCritical:
        lLogLevel = QStringLiteral("Critical");
        break;
      case ELogLevel::eLogImportant:
        lLogLevel = QStringLiteral("Important");
        break;
      case ELogLevel::eLogWarning:
        lLogLevel = QStringLiteral("Warning");
        break;
      case ELogLevel::eLogInfo:
        lLogLevel = QStringLiteral("Information");
        break;
      case ELogLevel::eLogEverything:
        lLogLevel = QStringLiteral("Debug");
        break;
    }
    QString lLogType;
    switch(eLogMessageType) {
      case ELogMessageType::eMessageReceived:
        lLogType = QStringLiteral("Message Received");
        break;
      case ELogMessageType::eMessageSent:
        lLogType = QStringLiteral("Message Sent");
        break;
      case ELogMessageType::eMemoryTransaction:
        lLogType = QStringLiteral("Memory Transaction");
        break;
      case ELogMessageType::eResourceManagement:
        lLogType = QStringLiteral("Resource Managment");
        break;
    }
    QTextStream lLogStream(&mLogFile);
    lLogStream << QDateTime::currentDateTime().time().toString("hh:mm:ss") << qSetFieldWidth(8) << mSeparator << lLogLevel << qSetFieldWidth(12) << left << lLogType << qSetFieldWidth(20) << left <<
                  lMessage << left;
  }
}
