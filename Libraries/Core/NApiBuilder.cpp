#include <QMetaMethod>
#include <QMetaObject>
#include "NApiBuilder.h"

namespace NulstarNS {
  NApiBuilder::NApiBuilder(QObject* rParent)
             : QObject(rParent) {

  }

  void NApiBuilder::fBuildApi() {
    if(pTargetObject != nullptr) {
        const QMetaObject* ametaObject = pTargetObject->metaObject();
        QStringList methods;
        for(int i = ametaObject->methodOffset(); i < ametaObject->methodCount(); ++i) {
             methods << QString::fromLatin1(ametaObject->method(i).methodSignature());
qDebug(ametaObject->method(i).methodSignature());
qDebug(QString::number(ametaObject->method(i).methodIndex()).toLatin1());
      }
qDebug(pTargetObject->objectName().toLatin1());
      int lFunctionIndex = pTargetObject->metaObject()->indexOfSlot("aFunction(int)");
qDebug(QString::number(lFunctionIndex).toLatin1());
      QMetaMethod lApiMethod = pTargetObject->metaObject()->method(lFunctionIndex);
qDebug(lApiMethod.tag());
/*qDebug(QString::number(lApiMethod.isValid()).toLatin1());
      sett.setValue("KEY1", lApiMethod.tag());*/
    }
  }
}
