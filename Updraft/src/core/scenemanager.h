#ifndef UPDRAFT_SRC_CORE_SCENEMANAGER_H_
#define UPDRAFT_SRC_CORE_SCENEMANAGER_H_

#include <QWidget>
#include <osg/ref_ptr>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/Viewpoint>
#include <QTimer>
#include <QHash>
#include <string>
#include "mapmanager.h"
#include "../mapobject.h"

namespace osgEarth {
namespace Util {
  class ElevationManager;
}
}

namespace Updraft {
namespace Core {

/// SceneManager class is a wrapper of the scene, and the scene graph.
class SceneManager: public QObject {
  Q_OBJECT

 public:
  SceneManager();
  ~SceneManager();

  /// Returns the drawing widget.
  /// \return The widget the scene is drawn into
  QWidget* getWidget();

  /// Returns the root of the scene.
  /// \return osg::Group object that represents the root of the scene.
  osg::Group* getRoot();

  /// Returns the map node in the scene.
  /// Note that there must be only one map node in the scene.
  /// \return Map node for the scene.
  osgEarth::MapNode* getMapNode();

  /// Returns the MapManager instance associated with currently
  /// displayed map.
  /// \return The MapManager instance for the currently displayed map.
  MapManager* getMapManager();

  /// Let MapManagers re-create the maps.
  void createMaps();

  /// Destroy all map nodes.
  void destroyMaps();

  /// Removes the group from the root.
  bool removeGroup(osg::Group* group);

  /// Returns pointer to the group node for random drawing.
  osg::Group* getSimpleGroup();

  /// Registers the osg node into Updraft for mouse picking.
  /// \param node The node that should be registered
  /// \param mapObject MapObject that represents this pickable node
  void registerOsgNode(osg::Node* node, MapObject* mapObject);

  /// Unregisters osg node for mouse picking
  /// \param node The node that should be unregistered
  void unregisterOsgNode(osg::Node* node);

  /// Tries to get the associated MapObject for the given node
  /// \param node The node whose MapObject we want
  /// \return Pointer to the MapObject that contains the registration info
  ///         for the given node, if the node was registered.
  MapObject* getNodeMapObject(osg::Node* node);

  /// Returns an elevation manager associated with the map
  /// that has elevation layer.
  /// \return pointer to the elevation manager object for the current map.
  osgEarth::Util::ElevationManager* getElevationManager();

  /// Returns the ellipsoid model associated with current
  /// active map.
  /// \return Ellipsoid model used for calculations on the current map.
  const osg::EllipsoidModel* getCurrentMapEllipsoid();

  /// Returns the osgEarth placer used to place nodes into the scene.
  /// \returns Object placer instance used to correctly place nodes onto the
  ///          map.
  osgEarth::Util::ObjectPlacer* getObjectPlacer();

  /// Returns the current viewpoint.
  /// \return The currently used viewpoint
  osgEarth::Util::Viewpoint getViewpoint();

 public slots:
  void redrawScene();
  void resetNorth();
  void untilt();
  void saveHomePosition();

 private slots:
  void checkedMap(bool value, MapLayerInterface* layer);

 private:
  /// Create map managers from earth files.
  void createMapManagers();

  /// Create the menu items in main window.
  void menuItems();

  /// Create the map layer group and fill it with maps.
  void mapLayerGroup();

  /// Return viewpoint that is either the same as the argument,
  /// or is moved above ground in case the argument was below.
  osgEarth::Util::Viewpoint correctedViewpoint(
    osgEarth::Util::Viewpoint viewpoint);

  osgEarth::Util::ElevationManager* createElevationManager();

  osgEarth::Util::ObjectPlacer* placer;

  osgViewer::Viewer* viewer;
  osg::Group* sceneRoot;

  /// Node with the background.
  osg::ClearNode* background;
  osgEarth::MapNode* mapNode;
  osg::Group* simpleGroup;

  QVector<MapManager*> mapManagers;
  QVector<MapLayerInterface*> layers;
  int activeMapIndex;
  osg::Camera* camera;
  MapManipulator* manipulator;
  osgEarth::Viewpoint saveViewpoint;
  osgEarth::Util::ElevationManager* elevationManager;
  osgQt::GraphicsWindowQt* graphicsWindow;

  /// Timer that triggers the drawing procedure.
  QTimer* timer;

  osg::GraphicsContext::Traits* createGraphicsTraits
    (int x, int y, int w, int h, const std::string& name = "",
    bool windowDecoration = false);
  osgQt::GraphicsWindowQt* createGraphicsWindow
    (osg::GraphicsContext::Traits* traits);
  osg::Camera* createCamera(osg::GraphicsContext::Traits* traits);

  /// Map of osg nodes registered for mouse picking
  QHash<osg::Node*, MapObject*> pickingMap;

  osgEarth::Viewpoint getHomePosition();
  osgEarth::Util::Viewpoint getInitialPosition();
  void startInitialAnimation();
  void updateCameraProjection();

  void insertMenuItems();

  double getAspectRatio();

  SettingInterface* homePositionSetting;
  static const float flyToHomeDuration;
};

}  // end namespace Core
}  // end namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SCENEMANAGER_H_
