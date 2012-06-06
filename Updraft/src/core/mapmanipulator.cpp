#include "mapmanipulator.h"
#include "updraft.h"
#include "../settinginterface.h"
#include "settingsmanager.h"

namespace Updraft {
namespace Core {

MapManipulator::MapManipulator() {
  Settings* settings = new Settings();
  bindKeyboardEvents(settings);
  bindMouseEvents(settings);
  settings->setLockAzimuthWhilePanning(true);
  applySettings(settings);

  mouseZoomSensitivity = updraft->settingsManager->addSetting(
    "map_advanced:mouse_zoom_sensitivity",
    tr("Mouse zoom sensitivity"),
    QVariant(1.0),
    true);

  mouseZoomSensitivity->callOnValueChanged(
    this, SLOT(mouseZoomSensitivityChanged()));
}

MapManipulator::~MapManipulator() {
  // We have to delete the interfaces to settings
  delete mouseZoomSensitivity;
}

void MapManipulator::bindKeyboardEvents(Settings* settings) {
  ActionOptions options;

  settings->bindKey(ACTION_HOME, osgGA::GUIEventAdapter::KEY_Space);

  // arrow keys: pan around
  settings->bindKey(ACTION_PAN_LEFT, osgGA::GUIEventAdapter::KEY_Left);
  settings->bindKey(ACTION_PAN_RIGHT, osgGA::GUIEventAdapter::KEY_Right);
  settings->bindKey(ACTION_PAN_UP, osgGA::GUIEventAdapter::KEY_Up);
  settings->bindKey(ACTION_PAN_DOWN, osgGA::GUIEventAdapter::KEY_Down);

  // A, shift + up arrow: zoom in
  settings->bindKey(ACTION_ZOOM_IN, osgGA::GUIEventAdapter::KEY_A);
  settings->bindKey(ACTION_ZOOM_IN, osgGA::GUIEventAdapter::KEY_Up,
    osgGA::GUIEventAdapter::MODKEY_SHIFT);

  // Z,Y, shift + down arrow: zoom out
  settings->bindKey(ACTION_ZOOM_OUT, osgGA::GUIEventAdapter::KEY_Z);
  settings->bindKey(ACTION_ZOOM_OUT, osgGA::GUIEventAdapter::KEY_Y);
  settings->bindKey(ACTION_ZOOM_OUT, osgGA::GUIEventAdapter::KEY_Down,
    osgGA::GUIEventAdapter::MODKEY_SHIFT);

  // ctrl + arrow keys: rotate
  settings->bindKey(ACTION_ROTATE_LEFT, osgGA::GUIEventAdapter::KEY_Left,
    osgGA::GUIEventAdapter::MODKEY_CTRL);
  settings->bindKey(ACTION_ROTATE_RIGHT, osgGA::GUIEventAdapter::KEY_Right,
    osgGA::GUIEventAdapter::MODKEY_CTRL);
  settings->bindKey(ACTION_ROTATE_UP, osgGA::GUIEventAdapter::KEY_Up,
    osgGA::GUIEventAdapter::MODKEY_CTRL);
  settings->bindKey(ACTION_ROTATE_DOWN, osgGA::GUIEventAdapter::KEY_Down,
    osgGA::GUIEventAdapter::MODKEY_CTRL);
}

void MapManipulator::bindMouseEvents(Settings* settings) {
  ActionOptions options;

  // left button: pan around
  settings->bindMouse(ACTION_PAN, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);

  // right (or ctrl+left) button: rotate
  settings->bindMouse(ACTION_ROTATE,
      osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON);
  settings->bindMouse(ACTION_ROTATE,
    osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON,
    osgGA::GUIEventAdapter::MODKEY_CTRL);

  // middle buttom (or alt+left): continous zoom
  options.clear();
  options.add(OPTION_CONTINUOUS, true);
  settings->bindMouse(ACTION_ZOOM,
    osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON, 0L, options);
  settings->bindMouse(ACTION_ZOOM,
    osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON,
    osgGA::GUIEventAdapter::MODKEY_ALT, options);

  // left + right buttom (or shift+left): continous panning
  options.clear();
  options.add(OPTION_CONTINUOUS, true);
  settings->bindMouse(ACTION_PAN,
    osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON
    | osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON,
    0L, options);
  settings->bindMouse(ACTION_PAN,
    osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON,
    osgGA::GUIEventAdapter::MODKEY_SHIFT, options);

  // scroll wheel: zoom
  settings->bindScroll(ACTION_ZOOM_IN, osgGA::GUIEventAdapter::SCROLL_UP);
  settings->bindScroll(ACTION_ZOOM_OUT, osgGA::GUIEventAdapter::SCROLL_DOWN);

  // ctrl + scroll wheel: tilt
  settings->bindScroll(ACTION_ROTATE_UP, osgGA::GUIEventAdapter::SCROLL_UP,
    osgGA::GUIEventAdapter::MODKEY_CTRL);
  settings->bindScroll(ACTION_ROTATE_DOWN, osgGA::GUIEventAdapter::SCROLL_DOWN,
    osgGA::GUIEventAdapter::MODKEY_CTRL);

  // shift + scroll wheel: rotate left/right
  settings->bindScroll(ACTION_ROTATE_LEFT, osgGA::GUIEventAdapter::SCROLL_UP,
    osgGA::GUIEventAdapter::MODKEY_SHIFT);
  settings->bindScroll(ACTION_ROTATE_RIGHT, osgGA::GUIEventAdapter::SCROLL_DOWN,
    osgGA::GUIEventAdapter::MODKEY_SHIFT);

  // double click with left button: zoom in on a point
  options.clear();
  options.add(OPTION_GOTO_RANGE_FACTOR, 0.4);
  settings->bindMouseDoubleClick(ACTION_GOTO,
    osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON, 0L, options);

  // double click with right (or ctrl+left): zoom out
  options.clear();
  options.add(OPTION_GOTO_RANGE_FACTOR, 2.5);
  settings->bindMouseDoubleClick(ACTION_GOTO,
    osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON, 0L, options);
  settings->bindMouseDoubleClick(ACTION_GOTO,
    osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON,
    osgGA::GUIEventAdapter::MODKEY_CTRL, options);
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
