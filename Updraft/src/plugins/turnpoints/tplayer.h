#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_

#include "tpfile.h"

namespace Updraft {

/// Class storing a turn-points layer.
class TPLayer {
 public:
  TPLayer(int layerId_, bool displayed_, const TPFile *file_);

  virtual ~TPLayer();

  /// \return Display state
  bool isDisplayed();

  /// Changes display state.
  /// \param displayed_ new value of a display state
  void display(bool displayed_);

 private:
  const int layerId;
  const TPFile *file;
  bool displayed;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPLAYER_H_
