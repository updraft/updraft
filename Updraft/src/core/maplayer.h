#ifndef UPDRAFT_SRC_CORE_MAPLAYER_H_
#define UPDRAFT_SRC_CORE_MAPLAYER_H_

#include <QObject>
#include "../maplayerinterface.h"

namespace Updraft {
namespace Core {

/// A class that represents one logical layer in map.
/// Map layer in osgEarth can be of different type, and this
/// class encapsulates all of the types, and creates common
/// interface.
/// The type of the map layer determines which value of the
/// union Layer is valid.
class MapLayer : public QObject, public MapLayerInterface {
  Q_OBJECT

 public:
  void connectSignalDisplayed(const QObject* receiver,
    const char *method);
  void connectSignalChecked(const QObject* receiver,
    const char *method);
  void connectSlotSetVisibility(const QObject* sender,
    const char *method);
  void connectCheckedToVisibility();

 private slots:
  void setVisibility(bool value);

 signals:
  void displayed(bool value, MapLayerInterface* sender);
  void checked(bool value, MapLayerInterface* sender);

 private:
  /// Emits the checked signal.
  /// Is called from MapLayerGroup.
  void emitChecked(bool value);
  void emitDisplayed(bool value);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_MAPLAYER_H_
