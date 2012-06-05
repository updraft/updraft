#ifndef UPDRAFT_SRC_CORE_ELLIPSOIDNAMEEDITOR_H_
#define UPDRAFT_SRC_CORE_ELLIPSOIDNAMEEDITOR_H_

#include <QComboBox>

#include "ellipsoidname.h"

namespace Updraft {
namespace Core {

class EllipsoidNameEditor: public QComboBox {
  Q_OBJECT
  Q_PROPERTY
    (Updraft::Core::EllipsoidName name READ name WRITE setName USER true)
 public:
  explicit EllipsoidNameEditor(QWidget *parent);

  EllipsoidName name();
  void setName(const EllipsoidName &n);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_ELLIPSOIDNAMEEDITOR_H_

