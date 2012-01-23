#ifndef UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_
#define UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_

#include <QtGui>

namespace Updraft {
namespace Core {

class SettingsDelegate: public QStyledItemDelegate {
 public:
  void setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const;
  void setEditorData(QWidget* editor, const QModelIndex& index) const;

 private:
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_

