#ifndef UPDRAFT_SRC_CORE_MAPMANIPULATOR_H_
#define UPDRAFT_SRC_CORE_MAPMANIPULATOR_H_

#include <osgEarthUtil/EarthManipulator>
#include <QObject>

namespace Updraft {

class SettingInterface;

namespace Core {

/// Earth manipulator supporting custom settings and keybindings.
class MapManipulator: public QObject, public osgEarth::Util::EarthManipulator {
  Q_OBJECT

 public:
  MapManipulator();
  ~MapManipulator();

  /// Runs the animation that resets this manipulator to the north.
  /// \param duration The requested duration of the animation
  void resetNorth(double duration);

  /// Runs the untilt animation on this manipulator.
  /// \param duration The requested duration of the animation
  void untilt(double duration);

  /// Updates the camera projection according to whether it should be 2D
  /// orthographic or 3D perspective.
  void updateCameraProjection();

 private slots:
  /// Handles the mouse zoom sensitivity setting change.
  void mouseZoomSensitivityChanged();

 private:
  /// The mouse zoom sensitivity setting.
  SettingInterface* mouseZoomSensitivity;

  /// Binds the osg keyboard events to the manipulator actions.
  /// \param settings The osg settings of this map manipulator.
  void bindKeyboardEvents(Settings* settings);

  /// Binds the osg mouse events to the manipulator actions.
  /// \param settings The osg settings of this map manipulator.
  void bindMouseEvents(Settings* settings);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPMANIPULATOR_H_
