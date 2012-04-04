#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPMAPOBJECT_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPMAPOBJECT_H_

#include "mapobject.h"
#include "turnpoint.h"

namespace Updraft {

class TPMapObject: public QObject, public MapObject {
  Q_OBJECT

 public:
  explicit TPMapObject(const TurnPoint* tp): QObject(NULL), MapObject(tp->name), turnPoint(tp) {}
  QObject* asQObject() { return this; }

  const TurnPoint* turnPoint;
};

}  // End namespace Updraft

#endif

