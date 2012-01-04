#ifndef OPENAIRSPACE_GLOBAL_H
#define OPENAIRSPACE_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore\qvector.h>
#include <QtCore\qfile.h>
#include <QtCore\qtextstream.h>
#include <QtCore\qlist.h>

#include "Airspace.h"

#ifdef OPENAIRSPACE_LIB
# define OPENAIRSPACE_EXPORT Q_DECL_EXPORT
#else
# define OPENAIRSPACE_EXPORT Q_DECL_IMPORT
#endif

#endif // OPENAIRSPACE_GLOBAL_H
