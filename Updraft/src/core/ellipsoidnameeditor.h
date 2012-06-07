#ifndef UPDRAFT_SRC_CORE_ELLIPSOIDNAMEEDITOR_H_
#define UPDRAFT_SRC_CORE_ELLIPSOIDNAMEEDITOR_H_

#include <QComboBox>

#include "ellipsoidname.h"

namespace Updraft {
namespace Core {

/// Editor for reference ellipsoid in settings dialog.
/// It is a combo box showing list of available ellipsoid models.
class EllipsoidNameEditor: public QComboBox {
  Q_OBJECT
  Q_PROPERTY
    (Updraft::Core::EllipsoidName name READ name WRITE setName USER true)
 public:
  explicit EllipsoidNameEditor(QWidget *parent);

  /// Return the selected ellipsoid's name.
  /// \return The name of the selected editor.
  EllipsoidName name();

  /// Sets the internal property of the editor.
  /// \param n The ellipsoid name to set.
  void setName(const EllipsoidName &n);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_ELLIPSOIDNAMEEDITOR_H_

