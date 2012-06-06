#ifndef UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_
#define UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_

#include <QtGui>

namespace Updraft {
namespace Core {

class SettingsBottomView;

class SettingsTopView: public QListView {
Q_OBJECT

 public:
  explicit SettingsTopView(QWidget* parent = 0);
  void setBottom(SettingsBottomView* b) { bottom = b; }

  void setModel(QAbstractItemModel* model);

 public slots:
  bool setShowAdvanced(bool show);

 protected slots:
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
  void rowsInserted(const QModelIndex& parent, int start, int end);
  void currentChanged(
    const QModelIndex& current, const QModelIndex& previous);

 private:
  /// Tells whether the group is a group with advanced settings
  bool groupIsAdvanced(int row);
  /// Says whether the group is hidden or empty and should thus not show
  bool groupShouldBeHidden(int row);

  /// Hides the settings group in the given row.
  void hideGroup(int row);
  /// Displays the settings grou in the given row.
  /// If the group does not contain any settings, it stays hidden.
  void displayGroup(int row);

  QRegExp advancedGroupRegExp;
  QRegExp hiddenGroupRegExp;
  bool showAdvanced;
  SettingsBottomView* bottom;
  bool groupChangeInProgress;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_

