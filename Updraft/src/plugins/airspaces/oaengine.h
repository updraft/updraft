#ifndef UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_
#define UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_

#include <QString>
#include <QtGui>
#include "../../pluginbase.h"

namespace Updraft {
namespace Airspaces {

class oaEngine {
 public:
   explicit oaEngine(MapLayerGroupInterface* LG) {mapLayerGroup = LG;}
   MapLayerInterface* Draw(const QString&);
  //bool Draw(const QString&);

 private:
     /// Map Layer Interface.
  MapLayerGroupInterface *mapLayerGroup;
  // QVector<MapLayerInterface*> mapLayers;
  QVector<QTreeWidgetItem*> treeItems;
};  // oaEngine

}  // Airspaces
}  // Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_