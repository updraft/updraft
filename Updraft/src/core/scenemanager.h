#ifndef UPDRAFT_SRC_CORE_SCENEMANAGER_H_
#define UPDRAFT_SRC_CORE_SCENEMANAGER_H_

#include <QtGui/QWidget>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/Viewpoint>
#include <QTimer>
#include <QHash>
#include <string>
#include "mapmanager.h"
#include "../mapobject.h"

namespace Updraft {
namespace Core {

/// SceneManager class is a wrapper of the scene, and the scene graph.
class SceneManager: public QObject {
  Q_OBJECT

 public:
  SceneManager();
  ~SceneManager();

  /// Returns the drawing widget.
  QWidget* getWidget();

  /// Returns the root of the scene.
  osg::Group* getRoot();

  /// Returns the map node in the scene.
  /// Note that there must be only one map node in the scene.
  osgEarth::MapNode* getMapNode();

  /// Returns the MapManager instance associated with the map.
  MapManager* getMapManager();

  /// Creates an empty osg::Group, and adds it to the root.
  /// There is always one group associated with one
  /// MapLayerGroup.
  osg::Group* newGroup();

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

 public slots:
  void redrawScene();
  void resetNorth();
  void untilt();

 private slots:
  void checkedMap(bool value, MapLayerInterface* layer);

 private:
  /// Create map managers from earth files.
  void createMapManagers();

  /// Create the menu items in main window.
  void menuItems();

  /// Create the map layer group and fill it with maps.
  void mapLayerGroup();

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

  void insertMenuItems();

  bool isCameraPerspective;

  double getAspectRatio();
  void updateCameraOrtho(osg::Camera* camera);
  void updateCameraPerspective(osg::Camera* camera);
};

}  // end namespace Core
}  // end namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SCENEMANAGER_H_
