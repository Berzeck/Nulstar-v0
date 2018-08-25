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
      NCoreService(QObject* rParent = nullptr);
      virtual ~NCoreService() {}

      virtual void fConnectToServiceManager(const QUrl& lUrl);

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
