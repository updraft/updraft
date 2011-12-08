#ifndef UPDRAFT_SRC_PLUGINS_AIRFIELDS_AIRFIELDS_H_
#define UPDRAFT_SRC_PLUGINS_AIRFIELDS_AIRFIELDS_H_

#include <QtGui>
#include "../../pluginapi.h"

namespace Updraft {
namespace Core {

class Q_DECL_EXPORT Airfields: public QObject, public IPlugin {
  Q_OBJECT
  Q_INTERFACES(Updraft::IPlugin)

 public:
  QString getName();

  unsigned getPriority();

  void initialize();

  void deinitialize();
 private:
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRFIELDS_AIRFIELDS_H_

