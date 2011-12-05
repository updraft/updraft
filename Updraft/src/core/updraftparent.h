#ifndef UPDRAFT_SRC_CORE_UPDRAFTPARENT_H_
#define UPDRAFT_SRC_CORE_UPDRAFTPARENT_H_

#include "pluginmanager.h"

namespace Updraft {
namespace Core {

/// Class that takes care of all the important objects in Updraft
class UpdraftParent {
 public:
  UpdraftParent();
  ~UpdraftParent() {}

 private:
  PluginManager pluginManager;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UPDRAFTPARENT_H_

