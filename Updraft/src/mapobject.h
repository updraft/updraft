#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_MAPOBJECT_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_MAPOBJECT_H_

#include <QString>
#include <QObject>

namespace Updraft {

class MapObject {
 public:
  /// Default constructor for use in QHash.
  /// Not to be used manually.
  MapObject() {}
  MapObject(QString objectName): name(objectName) {}

  /// Returns a pointer to this class typecasted to QObject.
  /// It should suffice to { return this; } in the implementation.
  /// \return Pointer to this object cast to QObject
  virtual QObject* asQObject() = 0;

  QString name;
};

}  // End namespace Updraft

#endif

