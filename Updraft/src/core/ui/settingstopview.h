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

 public slots:
  void toggleShowHidden();

 protected slots:
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
  void rowsInserted(const QModelIndex& parent, int start, int end);
  void currentChanged(
    const QModelIndex& current, const QModelIndex& previous);

 private:
  /// Tells whether the group is a group with hidden settings
  bool groupIsHidden(int row);

  QRegExp hiddenGroupRegExp;
  bool showHidden;
  SettingsBottomView* bottom;
  bool groupChangeInProgress;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_

