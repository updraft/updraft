#ifndef UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_
#define UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_

#include <QtGui>

namespace Updraft {
namespace Core {

class SettingsTopView: public QListView {
Q_OBJECT

 public:
  explicit SettingsTopView(QWidget* parent = 0);

 public slots:
  void toggleShowHidden();

 protected slots:
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
  void rowsInserted(const QModelIndex& parent, int start, int end);

 private:
  QRegExp hiddenGroupRegExp;
  bool showHidden;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_SETTINGSTOPVIEW_H_

