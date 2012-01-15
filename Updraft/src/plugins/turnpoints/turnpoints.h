#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_

#include <QtGui>
#include "../../pluginbase.h"

#include "../../libraries/cup/cup.h"

namespace Updraft {
namespace Core {

class Q_DECL_EXPORT TurnPoints : public QObject, public PluginBase {
  Q_OBJECT
  Q_INTERFACES(Updraft::PluginBase)

 public:
  QString getName();

  unsigned getPriority();

  void initialize();

  void deinitialize();

  bool fileOpen(QString filename, QList<int> roles);

  QStringList fileIdentification(QString filename);

 private:
  Cup::CupLoader cupLoader;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TURNPOINTS_H_
