#ifndef UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_
#define UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_

#include <QtGui>

namespace Updraft {
namespace Core {

class SettingsDelegate: public QStyledItemDelegate {
 public:
  explicit SettingsDelegate(QObject *parent = 0);

  QWidget* createEditor(QWidget* parent,
    const QStyleOptionViewItem& option, const QModelIndex& index) const;
  void setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const;
  void setEditorData(QWidget* editor, const QModelIndex& index) const;

 private:
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_

