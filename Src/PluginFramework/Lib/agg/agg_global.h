#ifndef AGG_GLOBAL_H
#define AGG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(AGG_LIBRARY)
#  define AGGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define AGGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // AGG_GLOBAL_H
