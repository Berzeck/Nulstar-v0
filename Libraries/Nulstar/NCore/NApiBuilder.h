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
      QVariantMap fApi() const { return mApi; }

    public Q_SLOTS:
      void fBuildApi(NCoreService* pTargetObject);

    protected:
      QVariantMap fBuildApiHeader(NCoreService* pTargetObject);

    private:
      QVariantMap mApi;

  };
}
#endif // NAPIBUILDER_H
