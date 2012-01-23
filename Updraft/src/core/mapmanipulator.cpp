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
  // lock azimuth -- north is always on the top
  getSettings()->setLockAzimuthWhilePanning(true);
}

}  // End namespace Core
}  // End namespace Updraft
