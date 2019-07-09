#ifndef NMODULEPARAMETER_H
#define NMODULEPARAMETER_H

#include <QString>

namespace NulstarNS {
  class NModuleParameter {
    public:
      explicit NModuleParameter();
      QString mGroupName;
      QString mParamName;
      QString mParamValue;
      inline NModuleParameter(const NModuleParameter& lModuleParam) {
        mGroupName = lModuleParam.mGroupName;
        mParamName = lModuleParam.mParamName;
        mParamValue = lModuleParam.mParamValue;
      }
      inline NModuleParameter &operator=(const NModuleParameter& lModuleParam) {
        mGroupName = lModuleParam.mGroupName;
        mParamName = lModuleParam.mParamName;
        mParamValue = lModuleParam.mParamValue;
        return *this;
      }

      inline bool operator==(const NModuleParameter& lModuleParam) {
        return (mGroupName == lModuleParam.mGroupName) && (mParamName == lModuleParam.mParamName) && (mParamValue == lModuleParam.mParamValue);
      }

   private:

  };
}

#endif // NMODULEPARAMETER_H
