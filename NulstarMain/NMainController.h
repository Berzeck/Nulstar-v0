#ifndef NMAINCONTROLLER_H
#define NMAINCONTROLLER_H

#include <QCommandLineOption>
#include <QHostAddress>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>
#include <NCoreService.h>
#include <NWebSocketServer.h>

namespace NulstarNS {
  class NMainController : public NCoreService {
    Q_OBJECT

    public:      
      explicit NMainController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lServiceManagerIP = QHostAddress::LocalHost,  QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry> (), quint16 lCommPort = 0,
                               QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
      ~NMainController() {}

      void fSetComponent(const QString& lComponentName, QList<QPair<QString, QString>> lParameters);

    private:      
      QMap<QString, QList<QPair<QString, QString>> > mComponents;

    public Q_SLOTS:
      void fStartComponent(const QString& lComponentName);
      void fStartComponents();
  };
}

#endif // NMainController_H
