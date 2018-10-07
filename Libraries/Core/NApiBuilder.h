#ifndef NAPIBUILDER_H
#define NAPIBUILDER_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include "Core.h"


namespace NulstarNS {
  class NWebSocketServer;
  class NCoreService;
  class CORESHARED_EXPORT NApiBuilder : public QObject {
    Q_OBJECT

    public:
      explicit NApiBuilder(QObject* rParent = nullptr);

    protected:
      QVariantMap fExtractHeader(NCoreService* pTargetObject);

    public Q_SLOTS:
      QVariantMap fBuildApi(NCoreService* pTargetObject);
  };
}
#endif // NAPIBUILDER_H
