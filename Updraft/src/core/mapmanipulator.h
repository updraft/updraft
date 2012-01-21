#ifndef UPDRAFT_SRC_CORE_MAPMANIPULATOR_H_
#define UPDRAFT_SRC_CORE_MAPMANIPULATOR_H_

#include <osgEarthUtil/EarthManipulator>

namespace Updraft {
namespace Core {

class MapManipulator: public osgEarth::Util::EarthManipulator {
 public:
  MapManipulator();
 protected:
  void handleMovementAction(const ActionType& type, double dx,
    double dy, osg::View* view);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPMANIPULATOR_H_
