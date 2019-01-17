#ifndef NMESSAGEFACTORY_H
#define NMESSAGEFACTORY_H

#include <QJsonObject>
#include <QObject>
#include <QStringList>

#include "NNetwork.h"

QT_FORWARD_DECLARE_CLASS(NMessage)

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageFactory : public QObject {
    Q_OBJECT

    public:
      explicit NMessageFactory(QObject* rParent = nullptr);
      static QJsonObject fMessageObjectFromString(const QString& lMessage, QString* rReturnMessageType = nullptr);
  };
}
#endif // NMESSAGEFACTORY_H
