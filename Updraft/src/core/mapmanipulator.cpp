#include "mapmanipulator.h"
#include "updraft.h"
#include "../settinginterface.h"
#include "settingsmanager.h"

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

  // Create a group for map settings
  /*
  updraft->settingsManager->addGroup(
    "map", tr("Map settings"), ":/core/icons/map.png");

  mouseZoomSensitivity = updraft->settingsManager->addSetting(
    "map:mouse_zoom_sensitivity",
    tr("Mouse zoom sensitivity"),
    QVariant(0.5));
  mouseZoomSensitivity->callOnValueChanged(
    this, SLOT(mouseZoomSensitivityChanged()));
  */
}

void MapManipulator::resetNorth(double duration) {
  osgEarth::Util::Viewpoint viewpoint = getViewpoint();
  viewpoint.setHeading(0);
  setViewpoint(viewpoint, duration);
}

void MapManipulator::untilt(double duration) {
  osgEarth::Util::Viewpoint viewpoint = getViewpoint();
  viewpoint.setPitch(-90);
  setViewpoint(viewpoint, duration);
}

void MapManipulator::mouseZoomSensitivityChanged() {
  getSettings()->setScrollSensitivity(mouseZoomSensitivity->get().toDouble());
}

void MapManipulator::updateCameraProjection() {
  bool isTilted = (getViewpoint().getPitch() > -88);
  bool isFar = (getDistance() > 7e6);

  osgEarth::Util::EarthManipulator::CameraProjection projection;
  if (isTilted || isFar) {
    projection = osgEarth::Util::EarthManipulator::PROJ_PERSPECTIVE;
  } else {
    projection = osgEarth::Util::EarthManipulator::PROJ_ORTHOGRAPHIC;
  }
  getSettings()->setCameraProjection(projection);
}

}  // End namespace Core
}  // End namespace Updraft
