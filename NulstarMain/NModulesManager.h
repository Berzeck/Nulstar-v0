#ifndef NMODULESMANAGER_H
#define NMODULESMANAGER_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>


namespace NulstarNS {
  class NModulesManager : public QObject {
    Q_OBJECT
    class NModuleInfo;
    public:      
      explicit NModulesManager(QObject *rParent = nullptr);
      ~NModulesManager() override { }


     NModuleInfo fModuleInfo(QString lModuleName, QString lVersion);


    private:      
        QMap<QString, QList<QPair<QString, QString>> > mModuleConfig;

        void fMoudlesReadConfig();

  };
}

#endif // NMODULESMANAGER_H
