#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QtCore/qglobal.h>

#if defined(CONNECTOR_LIBRARY)
#  define CONNECTORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONNECTORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CONNECTOR_H
