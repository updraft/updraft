#ifndef UPDRAFT_SRC_CORE_MAPLAYER_H_
#define UPDRAFT_SRC_CORE_MAPLAYER_H_

#include <QObject>
#include "../maplayerinterface.h"

namespace Updraft {
namespace Core {

class MapLayer : public QObject, public MapLayerInterface {
  Q_OBJECT

 public:
  MapLayer(int i, MapLayerType type, Layer layer);
  explicit MapLayer(int i);
  MapLayer(int i, osg::Node* node);
  MapLayer(int i, osgEarth::ImageLayer* imageLayer);
  MapLayer(int i, osgEarth::ElevationLayer* elevationLayer);
  MapLayer(int i, osgEarth::ModelLayer* modelLayer);
  ~MapLayer();

  void connectSignalDisplayed(const QObject* receiver,
    const char *method);
  void emitDisplayed(bool value);
  void setVisible(bool value);
  MapLayerType getType();
  Layer getLayer();
  void setLayer(Layer l);
  void setType(MapLayerType type);

 signals:
  void displayed(int id, bool value);

 private:
  MapLayerType layerType;
  Layer layer;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPLAYER_H_
