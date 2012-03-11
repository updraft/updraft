#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOT_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOT_H_

#include <QWidget>

namespace Updraft {
namespace IgcViewer {

class PlotWidget : public QWidget {
 private:
  void paintEvent(QPaintEvent* paintEvent);
}

}  // End namespace Updraft
}  // End namespace IgcViewer

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_PLOT_H_
