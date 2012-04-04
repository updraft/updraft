#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPMAPOBJECT_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPMAPOBJECT_H_

#include "mapobject.h"
#include "turnpoint.h"

#ifdef turnpoints_EXPORTS
# define TPS_EXPORT Q_DECL_EXPORT
#else
# define TPS_EXPORT Q_DECL_IMPORT
#endif

namespace Updraft {

class TPS_EXPORT TPMapObject: public QObject, public MapObject {
  Q_OBJECT

 public:
  explicit TPMapObject(const TurnPoint* tp): QObject(NULL),
    MapObject(tp->name), turnPoint(tp) {}
  QObject* asQObject() { return this; }

  const TurnPoint* turnPoint;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPMAPOBJECT_H_
