#ifndef UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_
#define UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_

#include <QtGui>

namespace Updraft {
namespace Core {

class SettingsBottomView;

/// List showing the setting groups.
/// It shows just the first item from the top-level rows of the data model.
/// These items describe, according to our convention, the setting groups.
class SettingsTopView: public QListView {
Q_OBJECT

 public:
  explicit SettingsTopView(QWidget* parent = 0);

  /// Sets the internal pointer to the bottom view.
  /// \param b The bottom view of the parent dialog.
  void setBottom(SettingsBottomView* b) { bottom = b; }

  /// Sets the data model used by this view.
  /// \param model The model used for retrieving the setting group data.
  void setModel(QAbstractItemModel* model);

 public slots:
  /// Shows/hides the advanced setting groups.
  /// \param show Whether to show the advanced setting groups.
  bool setShowAdvanced(bool show);

 protected slots:
  /// Handles change of data in some items. Updates the displayed data.
  /// This slot should be connected to the corresponding signal in the model.
  /// Note that this view does not support multiple data items changed in one
  /// signal, so topLeft should always equal to bottomRight.
  /// \param topLeft Index of the top-left item from the set that changed.
  /// \param bottomRight Index of the bottom-rigt item from the set that
  ///                    changed.
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

  /// Handles row insertion into the model. Updates the displayed data.
  /// This slot should be connected to the corresponding signal in the model.
  /// \param parent Index of the item under which the rows were inserted.
  /// \param start The first row that was inserted.
  /// \param end Index one-after the last inserted row.
  void rowsInserted(const QModelIndex& parent, int start, int end);

  /// Handles change of the currently selected item.
  /// Updates the displayed data.
  /// This slot should be connected to the corresponding signal in the model.
  /// \param current The new currently selected index.
  /// \param previous The previously selected index.
  void currentChanged(
    const QModelIndex& current, const QModelIndex& previous);

 private:
  /// Tells whether the group is a group with advanced settings.
  /// \param row The row number of the group.
  /// \return Whether the group is an advanced group.
  bool groupIsAdvanced(int row);

  /// Says whether the group is hidden or empty and should thus not show
  /// \param row The row number of the group.
  /// \return Whether the group is a hidden group or an empty group. Both cases
  ///         mean that it should not be displayed in the dialog.
  bool groupShouldBeHidden(int row);

  /// Hides the settings group in the given row.
  /// \param row The row number of the group.
  void hideGroup(int row);

  /// Displays the settings grou in the given row.
  /// If the group does not contain any settings, it stays hidden.
  /// \param row The row number of the group.
  void displayGroup(int row);

  /// Regular expression that determines whether the group is an advanced group
  QRegExp advancedGroupRegExp;

  /// Regular expression that determines whether the group is a hidden group
  QRegExp hiddenGroupRegExp;

  /// Whether the advanced groups should be shown
  bool showAdvanced;

  /// Pointer to the bottom view
  SettingsBottomView* bottom;

  /// Helper flag that prevents re-entry in the currentChanged() method.
  bool groupChangeInProgress;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_

