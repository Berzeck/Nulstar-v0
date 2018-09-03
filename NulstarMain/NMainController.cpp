#include <QHostAddress>
#include <QProcess>
#include <QSysInfo>
#include <QTimer>
#include <QWebSocket>
#include "NMainController.h"

const QString lComponentsDirectory("MainComponents");

namespace NulstarNS {
  NMainController::NMainController(QWebSocketServer::SslMode lSslMode, ELogLevel lLogLevel, QList<QNetworkAddressEntry> lAllowedNetworks, QObject* rParent)
                 : NCoreService(lLogLevel, lAllowedNetworks, rParent) {
    NCoreService::mLogLevel = lLogLevel;
    pWebCommServer = new NWebSocketServer(QStringLiteral("Nulstar Internal Communication"), lSslMode, this);
    QTimer::singleShot(0, this, &NMainController::fStartComponents);
  }

  NMainController::~NMainController() {
    pWebCommServer->close();
  }

  void NMainController::fControlCommServer(EServiceAction lAction, quint16 lCommPort, QHostAddress::SpecialAddress lAddress ) {
    if(lAction == EServiceAction::eStartService) {
      pWebCommServer->fListen(lAddress, lCommPort);
    }
    if(lAction == EServiceAction::eStopService) {
      pWebCommServer->close();
    }
    if(lAction == EServiceAction::eChangePort) {
      pWebCommServer->fSetPort(lCommPort);
    }
  }

  void NMainController::fSetComponent(const QString& lComponentName, QList<QPair<QString, QString> > lParameters) {
    mComponents[lComponentName] = lParameters;
  }

  void NMainController::fStartComponent(const QString& lComponentName) {
    QString lAppName(QString("%1/%2").arg(lComponentsDirectory).arg(lComponentName));
    if(QSysInfo::productType() == QString("windows")) lAppName.append(".exe");

    QStringList lParameterList;
    for(const QPair<QString, QString>& lParameter : mComponents.value(lComponentName)) {
      QString lFormattedParameter(lParameter.first);
      lFormattedParameter.prepend("--");
      lParameterList << lFormattedParameter << lParameter.second;
    }
    //QProcess::startDetached(lAppName)
  }

  void NMainController::fStartComponents() {
    QStringList lAllComponents(mComponents.keys());
    for(const QString& lComponentName : lAllComponents)
      fStartComponent(lComponentName);
  }
}
