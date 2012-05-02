#ifndef UPDRAFT_SRC_PLUGINS_IGCVIEWER_COLORINGS_H_
#define UPDRAFT_SRC_PLUGINS_IGCVIEWER_COLORINGS_H_

#include <QColor>

#include "util/util.h"

#include "igcinfo.h"

namespace Updraft {
namespace IgcViewer {

/// Coloring is a class that gives colors to points in igc track recordings.
class Coloring {
 public:
  virtual ~Coloring() {}

  /// Return the color for vertex.
  /// \param i Index of the fix.
  virtual QColor color(int i) = 0;
};

/// Default coloring just adds together gradient and scaled info.
class DefaultColoring : public Coloring {
 public:
  DefaultColoring(const IgcInfo *info, const Util::Gradient *gradient);
  QColor color(int i);

 protected:
  const IgcInfo *info;
  const Util::Gradient *gradient;
};

/// Same as default coloring, but uses the scale symmetrically.
class SymmetricColoring : public DefaultColoring {
 public:
  SymmetricColoring(const IgcInfo *info, const Util::Gradient *gradient);
  QColor color(int i);
};

/// Same as default coloring, but doesn't use global scale.
class LocalColoring : public DefaultColoring {
 public:
  LocalColoring(const IgcInfo *info, const Util::Gradient *gradient);
  QColor color(int i);
};

/// Coloring that returns a constant color.
class ConstantColoring : public Coloring {
 public:
  explicit ConstantColoring(QColor color);
  QColor color(int i);

 protected:
  QColor c;
};

}  // End namespace IgcViewer
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_IGCVIEWER_COLORINGS_H_

