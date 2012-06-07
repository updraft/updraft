#ifndef UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_
#define UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_

#include <QtGui>

namespace Updraft {
namespace Core {

/// This class allows us to override the default editors for setting items.
class SettingsDelegate: public QStyledItemDelegate {
 public:
  explicit SettingsDelegate(QObject *parent = 0);

  /// Created an editor for the setting at the given model index.
  /// \param parent The parent that should be set in the editor.
  /// \param option Style option for the item at the given index.
  /// \param index The model index of the setting for which the editor should
  ///              be created.
  QWidget* createEditor(QWidget* parent,
    const QStyleOptionViewItem& option, const QModelIndex& index) const;

  /// Trensfers the data from the item editor to the data model.
  /// \param editor Pointer to the editor widget for this setting's item.
  /// \param model Pointer to the model containing the setting.
  /// \param index Index at which the setting resides in the model.
  void setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const;

  /// Transfers the data from the model to the editor.
  /// \param editor The editor for this item that was previously created with
  ///               the createEditor() method.
  /// \param index The model index of the settings item whose data should be
  ///              transfered.
  void setEditorData(QWidget* editor, const QModelIndex& index) const;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSDELEGATE_H_

