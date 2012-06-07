#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_MAPOBJECT_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_MAPOBJECT_H_

#include <QString>
#include <QObject>

namespace Updraft {

/// Base class for objects that can be clicked on in the map.
class MapObject {
 public:
  /// Default constructor for use in QHash.
  /// Not to be used manually.
  MapObject() {}
  MapObject(QString objectName): name(objectName) {}

  /// Returns a name of the class for runtime type identification.
  virtual QString getObjectTypeName() = 0;

  QString name;
};

}  // End namespace Updraft

#endif
