#ifndef NMODULEINFO_H
#define NMODULEINFO_H

#include <QString>
#include <QStringList>

namespace NulstarNS {
  class NModuleInfo{

    public:
      QString mModuleAppName;
      QStringList mModuleParametersList;
      QString mModuleEnvLibsPath;
      QString mModuleWorkingDirectory;
  };
}

#endif // NMODULEINFO_H
