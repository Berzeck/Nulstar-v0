#ifndef NCORESERVICE_H
#define NCORESERVICE_H

#include <QObject>
#include <QUrl>
#include <QWebSocket>
#include "Network.h"

namespace NulstarNS {  
  class NETWORKSHARED_EXPORT NCoreService : public QObject {
    Q_OBJECT

    public:
      enum class ELogLevel {eLogCritical = 1, eLogImportant = 2, eLogWarning = 3, eLogInfo = 4, eLogEverything = 5};
      enum class EServiceAction {eStartService = 0, eStopService = 1, eRestartService = 2, eChangePort = 3};

      NCoreService(ELogLevel lLogLevel = ELogLevel::eLogWarning, QObject* rParent = nullptr);
      virtual ~NCoreService() {}

      virtual void fConnectToServiceManager(const QUrl& lUrl);

    protected:
      ELogLevel mLogLevel;

    private:      
      QWebSocket mWebSocket;

    private Q_SLOTS:
      void fOnConnected();
      void fOnTextMessageReceived(const QString& lTextMessage);

    Q_SIGNALS:
      void sClosed();
  };
}

#endif // NCORESERVICE_H
