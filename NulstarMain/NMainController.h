#ifndef NMAINCONTROLLER_H
#define NMAINCONTROLLER_H

#include "../AppVersion.h"

#include <NCoreService.h>
#include <NWebSocketServer.h>
#include <QCommandLineOption>
#include <QHostAddress>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QUrl>

namespace NulstarNS {
class NMainController : public NCoreService {
    Q_OBJECT

public:
    explicit NMainController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QUrl& lServiceManagerUrl = QUrl(), QList<QNetworkAddressEntry> lAllowedNetworks = QList<QNetworkAddressEntry>(), quint16 lCommPort = 0,
        QHostAddress::SpecialAddress lBindAddress = QHostAddress::Null, QObject* rParent = nullptr);
    ~NMainController() override {}

    QString fName() const override { return QString(APP_NAME); }
    QString fVersion() const override { return QString(APP_VERSION); }
    QString fDomain() const override { return QString(APP_DOMAIN); }
    QString fApiRole() const override { return QString(APP_ROLE); }
    QString fApiVersion() const override { return QString(APP_ROLE_VERSION); }

    void fSetComponent(const QString& lComponentName, QList<QPair<QString, QString>> lParameters);

private:
    QMap<QString, QList<QPair<QString, QString>>> mComponents;

public Q_SLOTS:
    void fStartComponent(const QString& lComponentName) const;
    void fStartComponents();
};
}

#endif // NMainController_H
