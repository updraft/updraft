#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_MAPOBJECT_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_MAPOBJECT_H_

#include <QString>
#include <QObject>

namespace Updraft {

class MapObject: public QObject {
  Q_OBJECT

  public:
    /// Default constructor for use in QHash.
    /// Not to be used manually.
    MapObject() {}
    MapObject(QString objectName): name(objectName) {}

    QString name;
};

}  // End namespace Updraft

#endif

