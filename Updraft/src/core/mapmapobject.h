#ifndef UPDRAFT_SRC_CORE_MAPMAPOBJECT_H_
#define UPDRAFT_SRC_CORE_MAPMAPOBJECT_H_

#include "../mapobject.h"

namespace Updraft {
namespace Core {

/// Map object of the globe surface.
/// This is usesd to implement clicking on the map.
class MapMapObject : public MapObject {
 public:
  MapMapObject(): MapObject("Terrain") {}

  // Use this method for comparing class name.
  // \return Name of this class.
  static QString getClassName() { return "MapMapObject"; }

  QString getObjectTypeName() { return getClassName(); }
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPMAPOBJECT_H_

