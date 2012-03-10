#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_GLOBAL_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_GLOBAL_H_

#include <QtCore/qglobal.h>
#include <QtCore/qvector.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qlist.h>
#include <QPair>
#include <QString>
#include <QRegExp>


#ifdef UPDRAFT_OPENAIRSPACE_INTERNAL
# define OPENAIRSPACE_EXPORT Q_DECL_EXPORT
#else
# define OPENAIRSPACE_EXPORT Q_DECL_IMPORT
#endif

#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_GLOBAL_H_
