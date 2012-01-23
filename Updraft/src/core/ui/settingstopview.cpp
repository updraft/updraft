#include "settingstopview.h"

namespace Updraft {
namespace Core {

SettingsTopView::SettingsTopView(QWidget* parent): QListView(parent) {
  hiddenGroupRegExp = QRegExp("_hidden$");
  showHidden = false;
}

void SettingsTopView::toggleShowHidden() {
  showHidden = !showHidden;

  if (showHidden) {
    for (int row = 0; row < model()->rowCount(); ++row) {
      setRowHidden(row, false);
    }
  } else {
    for (int row = 0; row < model()->rowCount(); ++row) {
      QModelIndex groupIndex = model()->index(row, 0);
      QVariant groupName = model()->data(groupIndex, Qt::UserRole);
      // The group name has "_hidden" at the end
      if (hiddenGroupRegExp.indexIn(groupName.toString()) != -1) {
        setRowHidden(row, true);
      }
    }
  }
}

void SettingsTopView::dataChanged(
  const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  QModelIndex parent = topLeft.parent();

  // If groups were modified, check whether they should be hidden
  if (!showHidden && !parent.isValid()) {
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
      QModelIndex groupIndex = model()->index(row, 0, parent);
      QVariant groupName = model()->data(groupIndex, Qt::UserRole);
      // The group name has "_hidden" at the end
      if (hiddenGroupRegExp.indexIn(groupName.toString()) != -1) {
        setRowHidden(row, true);
      }
    }
  }

  QListView::dataChanged(topLeft, bottomRight);
}

void SettingsTopView::rowsInserted(
  const QModelIndex& parent, int start, int end) {}

}  // End namespace Core
}  // End namespace Updraft

