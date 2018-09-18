#ifndef NAPIBUILDER_H
#define NAPIBUILDER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include "Core.h"


namespace NulstarNS {
  class NWebSocketServer;
  class NCoreService;
  class CORESHARED_EXPORT NApiBuilder : public QObject {
    Q_OBJECT

    public:
      explicit NApiBuilder(QObject* rParent = nullptr);

      void fSetTargetObject(NCoreService* rTargetObject) { pTargetObject = rTargetObject; }

   protected:
      QMap<QString, QVariant> fExtractHeader();

    private:
      NCoreService* pTargetObject;

    public Q_SLOTS:
      QMap<QString, QVariant> fBuildApi();
  };
}
#endif // NAPIBUILDER_H
