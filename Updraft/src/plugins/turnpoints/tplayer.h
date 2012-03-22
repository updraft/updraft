#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_

#include <osg/Matrix>
#include "tpfile.h"
#include "mapobject.h"

namespace osg {
  class Geometry;
  class Geode;
  class AutoTransform;
  class Group;
  class Node;
}

namespace osgEarth {
namespace Util {
  class ObjectPlacer;
}
}

namespace Updraft {

// TODO(cestmir): This is just a temporary class to test mouse picking
// Remove it as soon as real turnpoint map objects are implemented
class TPMapObject: public QObject, public MapObject {
  Q_OBJECT

 public:
  explicit TPMapObject(const QString& name): QObject(NULL), MapObject(name) {}
  QObject* asQObject() { return this; }
};

class TurnPoints;

/// Class storing a turn-points layer.
class TPLayer {
 public:
  TPLayer(bool displayed_, osgEarth::Util::ObjectPlacer* objectPlacer_,
    const TPFile *file_, const QString &dataDir, TurnPoints* parent_);

  virtual ~TPLayer();

  /// \return osgNode associated with the layer.
  osg::Node* getNode() const;

  /// \return Display state
  bool isDisplayed();

  /// Changes display state.
  /// \param displayed_ new value of a display state
  void display(bool displayed_);

 private:
  /// Creates osg::Geometry for turn-point billboard.
  /// \param scale relative size of the node.
  /// \return A new instance of osg::Geometry
  osg::Geometry* createGeometry(qreal scale);

  /// Creates osg::Geometry for turn-point billboard.
  /// \param scale relative size of the node.
  /// \return A new instance of osg::Geode
  osg::Geode* createGeode(qreal scale);

  /// Creates osg::AutoTransform for turn-point billboard.
  /// This function is called for each TP.
  /// \param matrix transformation of a TP
  /// \param geode node used for object visualization
  /// \return A new instance of osg::AutoTransform
  osg::AutoTransform* createAutoTransform(
    const osg::Matrix& matrix,
    osg::Geode* geode);

  /// osg Node representing this turn-points layer
  osg::Group* group;

  /// osgEarth placer for placing objects to specific geo.coordinates
  osgEarth::Util::ObjectPlacer* objectPlacer;

  const TPFile *file;
  bool displayed;

  // Path to application data directory.
  QString dataDir;

  TurnPoints* parent;

  // TODO(cestmir): Serves for destructor purposes. Remove in future
  // when real turnpoint MapObjects are implemented
  QList<TPMapObject*> mapObjects;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_
