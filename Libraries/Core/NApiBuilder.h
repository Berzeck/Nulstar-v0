#ifndef NAPIBUILDER_H
#define NAPIBUILDER_H

#include <QObject>
#include "Core.h"

namespace NulstarNS {
  class NWebSocketServer;
  class CORESHARED_EXPORT NApiBuilder : public QObject {
    Q_OBJECT

    public:
      explicit NApiBuilder(QObject* rParent = nullptr);

      void fSetTargetObject(QObject* rTargetObject) { pTargetObject = rTargetObject; }

    private:
      QObject* pTargetObject;

    public Q_SLOTS:
      virtual void fBuildApi();
  };
}
#endif // NAPIBUILDER_H
