#include "mapmanipulator.h"

namespace Updraft {
namespace Core {

MapManipulator::MapManipulator() {
  // switch the scroll wheel
  getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_IN,
    osgGA::GUIEventAdapter::SCROLL_UP);
  getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_OUT,
    osgGA::GUIEventAdapter::SCROLL_DOWN);
  // TODO(Maria): change the earth rotating
  // getSettings()->bindMouse(
    // osgEarth::Util::EarthManipulator::ACTION_ROTATE,
    // osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
  // lock azimuth -- north is always on the top
  getSettings()->setLockAzimuthWhilePanning(true);
}

void MapManipulator::handleMovementAction(const ActionType& type, double dx,
  double dy, osg::View* view) {
  switch (type) {
    case ACTION_PAN:
      pan(dx, dy);
      break;
    case ACTION_ROTATE:
      // in "single axis" mode, zero out one of the deltas.
      /* if (_continuous && _settings->getSingleAxisRotation()) {
        if (::fabs(dx) > ::fabs(dy))
          dy = 0.0;
        else
          dx = 0.0;
      } */
      rotate(dx, dy);
      break;
    case ACTION_ZOOM:
      zoom(dx, dy);
      break;
    case ACTION_EARTH_DRAG:
      drag(dx, dy, view);
      break;
    default:
      (void)0;  // silence the warning
  }
}

}  // End namespace Core
}  // End namespace Updraft
