#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_

#include "tpfile.h"

namespace osg {
  class Billboard;
  class Geometry;
}

namespace osgEarth {
namespace Util {
  class ObjectPlacer;
}
}

namespace Updraft {

/// Class storing a turn-points layer.
class TPLayer {
 public:
  TPLayer(bool displayed_, osgEarth::Util::ObjectPlacer* objectPlacer_,
    const TPFile *file_, const QString &dataDir);

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
  /// \param scale relative size of the billboard
  /// \return A new instance of osf::Drawable
  osg::Geometry* createGeometry(qreal scale);

  /// osg Node representing this turn-points layer
  osg::Billboard* billboard;

  /// osgEarth placer for placing objects to specific geo.coordinates
  osgEarth::Util::ObjectPlacer* objectPlacer;

  const TPFile *file;
  bool displayed;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_
