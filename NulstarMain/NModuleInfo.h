#ifndef NMODULEINFO_H
#define NMODULEINFO_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>

namespace NulstarNS {
  class NModuleInfo : public QObject {
    Q_OBJECT

    public:
      explicit NModuleInfo(QObject *rParent = nullptr);
      ~NModuleInfo() override { }

      QString mModuleAppName;
      QString mModuleParameters;
      QString mModuleEnvLibsPath;
      QString mModuleWorkingDirectory;
  };
}

#endif // NMODULEINFO_H
