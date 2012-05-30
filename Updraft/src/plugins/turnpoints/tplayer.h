#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_

#include <osg/Geometry>
#include <osg/Matrix>
#include "tpfile.h"
#include "tpmapobject.h"
#include "../../pluginbase.h"

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

class TurnPoints;

/// Class storing a turn-points layer.
class TPLayer {
 public:
  TPLayer(bool displayed_, osgEarth::Util::ObjectPlacer* objectPlacer_,
    const TPFile *file_, const QString &dataDir, TurnPoints* parent_,
    const QVector<SettingInterface*>& settings);

  virtual ~TPLayer();

  /// \return osgNode associated with the layer.
  osg::Node* getNode() const;
  osg::Node* getLblNode() const;

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
  osg::Geode* createGeode(qreal scale, bool isAirfield);

  /// Creates osg::AutoTransform for turn-point billboard.
  /// This function is called for each TP.
  /// \param matrix transformation of a TP
  /// \param geode node used for object visualization
  /// \return A new instance of osg::AutoTransform
  osg::AutoTransform* createAutoTransform(
    const osg::Matrix& matrix,
    osg::Geode* geode);

  /// Autoscale creates the osg::Node Label
  osg::Node* createAutoScale(
    const osg::Vec3& position,
    qreal characterSize,
    const QString& message,
    qreal minScale = 0.0,
    qreal maxScale = FLT_MAX);

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

  /// Text min/max visibility setting
  qreal labelMinScale;
  qreal labelMaxScale;

  /// Labels display distance from camera.
  /// This value is the distance that LOD starts
  /// to display the labels.
  /// Reasonable value : 4000 (default)
  qreal labelDrawDist;

  /// Label font size
  qreal lblSize;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_
