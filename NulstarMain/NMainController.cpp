#include <QCoreApplication>
#include <QHostAddress>
#include <QProcess>
#include <QTimer>
#include <QWebSocket>
#include "NMainController.h"

const QString lComponentsDirectory("MainComponents");

namespace NulstarNS {
  NMainController::NMainController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, const QHostAddress& lServiceManagerIP, QList<QNetworkAddressEntry> lAllowedNetworks, quint16 lCommPort, QHostAddress::SpecialAddress lBindAddress, QObject* rParent)
                 : NCoreService(lSslMode, lLogLevel, lServiceManagerIP, lAllowedNetworks, lCommPort, lBindAddress, rParent) {
    QTimer::singleShot(0, this, &NMainController::fStartComponents);
  }


  void NMainController::fSetComponent(const QString& lComponentName, QList<QPair<QString, QString> > lParameters) {
    mComponents[lComponentName] = lParameters;
  }

  void NMainController::fStartComponent(const QString& lComponentName) {
    QString lAppName(QString("%1/%2/%3").arg(QCoreApplication::applicationDirPath()).arg(lComponentsDirectory).arg(lComponentName));
 //   QString lSystem(QSysInfo::productType());
#ifdef Q_OS_WIN
  lAppName.append(".exe");
#endif

    QStringList lParameterList;
    for(const QPair<QString, QString>& lParameter : mComponents.value(lComponentName)) {
      QString lFormattedParameter(lParameter.first);
      lFormattedParameter.prepend("--");
      lParameterList << lFormattedParameter << lParameter.second;
    }
    bool lServiceExecuted(QProcess::startDetached(lAppName, lParameterList, QString("%1/%2/").arg(QCoreApplication::applicationDirPath()).arg(lComponentsDirectory)));
    if(!lServiceExecuted) {
qDebug("Execution failed!");
    }
  }

  void NMainController::fStartComponents() {
    QStringList lAllComponents(mComponents.keys());
    for(const QString& lComponentName : lAllComponents)
      fStartComponent(lComponentName);
  }
}
