#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_MAPOBJECT_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_MAPOBJECT_H_

#include <osg/Referenced>
#include <QString>

namespace Updraft {

class MapObject {
  public:
    /// Default constructor for use in QHash.
    /// Not to be used manually.
    MapObject() {}
    MapObject(osg::Node* node, QString objectName): name(objectName) {}

    QString name;
};

}  // End namespace Updraft

#endif

