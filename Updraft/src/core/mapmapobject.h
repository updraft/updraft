#ifndef UPDRAFT_SRC_CORE_MAPMAPOBJECT_H_
#define UPDRAFT_SRC_CORE_MAPMAPOBJECT_H_

#include "../mapobject.h"

namespace Updraft {
namespace Core {

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

