#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_

#include <QtCore/QtGlobal>
#include <QString>
#include <QList>

#ifdef OPENAIRSPACE_LIB
# define OPENAIRSPACE_EXPORT Q_DECL_EXPORT
#else
# define OPENAIRSPACE_EXPORT Q_DECL_IMPORT
#endif

void OPENAIRSPACE_EXPORT TTT();

namespace OpenAirspace {
  class OPENAIRSPACE_EXPORT Parser {
 public:
    void Test();
  };  // Parser
}  // OpenAirspace
#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
