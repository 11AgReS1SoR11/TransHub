#ifndef MATRIX_GLOBAL_H
#define MATRIX_GLOBAL_H

//#include <qglobal.h>

#if defined(MATRIX_LIBRARY)
#  define MATRIX_EXPORT Q_DECL_EXPORT
#else
#  define MATRIX_EXPORT Q_DECL_IMPORT
#endif

#endif // MATRIX_GLOBAL_H
