#ifndef NTOOLS_H
#define NTOOLS_H

#include <QtCore/qglobal.h>

#if defined(NTOOLS_LIBRARY)
#  define NTOOLSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NTOOLSSHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // NTOOLS_H
