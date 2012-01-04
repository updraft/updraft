#ifndef UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_
#define UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_

#include <QtGui>

namespace Updraft {
namespace Core {

class SettingsDelegate: public QStyledItemDelegate {
 public:
  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

 private:
  bool variantsEqual(const QVariant& var1, const QVariant& var2) const;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_

