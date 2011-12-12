#include "mapmanipulator.h"

MapManipulator::MapManipulator () {
  // switch the scroll wheel
  getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_IN, osgGA::GUIEventAdapter::SCROLL_UP); 
  getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_OUT, osgGA::GUIEventAdapter::SCROLL_DOWN);
  // change the earth rotating
  getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_ROTATE, osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON);                      
  // lock azimuth -- north always on the top
  getSettings()->setLockAzimuthWhilePanning(true);
}