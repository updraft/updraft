#include "settingstopview.h"
#include "settingsbottomview.h"

namespace Updraft {
namespace Core {

SettingsTopView::SettingsTopView(QWidget* parent): QListView(parent) {
  bottom = NULL;
  hiddenGroupRegExp = QRegExp("_hidden$");
  showHidden = false;
  groupChangeInProgress = false;
  setSelectionMode(QAbstractItemView::SingleSelection);
}

void SettingsTopView::toggleShowHidden() {
  // If the currently selected group should be hidden, try to change it
  if (showHidden && groupIsHidden(currentIndex().row())) {}

  showHidden = !showHidden;

  if (showHidden) {
    for (int row = 0; row < model()->rowCount(); ++row) {
      setRowHidden(row, false);
    }
  } else {
    for (int row = 0; row < model()->rowCount(); ++row) {
      if (groupIsHidden(row)) {
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

bool SettingsTopView::groupIsHidden(int row) {
  QModelIndex groupIndex = model()->index(row, 0);
  QVariant groupName = model()->data(groupIndex, Qt::UserRole);
  // The group name has "_hidden" at the end
  return hiddenGroupRegExp.indexIn(groupName.toString()) != -1;
}

void SettingsTopView::currentChanged(
  const QModelIndex& current,
  const QModelIndex& previous) {
  if (groupChangeInProgress) return;
  if (!bottom) return;
  if (bottom->editorValuesChanged()) {
    QMessageBox::StandardButton butt =
      QMessageBox::question(NULL, "Unsaved changes in this tab.",
      "You have some unsaved changes to the setting values in the current "
      "settings group. Do you want to apply these changes or reset the "
      "settings to their previous values?",
      QMessageBox::Reset | QMessageBox::Apply | QMessageBox::Cancel);
    switch (butt) {
      case QMessageBox::Reset:
        bottom->reset();
        bottom->setTopIndex(current);
      break;
      case QMessageBox::Apply:
        bottom->commit();
        bottom->setTopIndex(current);
      break;
      default:
        groupChangeInProgress = true;
        setCurrentIndex(previous);
        groupChangeInProgress = false;
        return;
      break;
    }
  } else {
    bottom->setTopIndex(current);
  }
}

}  // End namespace Core
}  // End namespace Updraft

