#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPMAPOBJECT_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPMAPOBJECT_H_

#include "mapobject.h"
#include "turnpoint.h"

namespace Updraft {

/// The turnpoint map object that serves for clicking at turnpoints
class TPMapObject : public MapObject {
 public:
  explicit TPMapObject(const TurnPoint* tp)
  : MapObject(tp->name), turnPoint(tp) {}

  // Use this method for comparing class name.
  // \return Name of this class.
  static QString getClassName() { return "TPMapObject"; }

  QString getObjectTypeName() { return getClassName(); }

  const TurnPoint* turnPoint;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPMAPOBJECT_H_
