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

void SettingsTopView::setModel(QAbstractItemModel* model) {
  QAbstractItemView::setModel(model);

  for (int row = 0; row < model->rowCount(); ++row) {
    if (groupIsEmpty(row)) {
      hideGroup(row);
    }
  }
}

bool SettingsTopView::setShowHidden(bool show) {
  // If the currently selected group should be hidden, try to change it
  if (currentIndex().isValid() &&
    showHidden &&
    !show &&
    groupIsHidden(currentIndex().row())) {
    // Find the first row that doesn't have to be hidden and try to select it
    int row;
    for (row = 0; row < model()->rowCount(); ++row) {
      if (!groupIsHidden(row)) break;
    }
    setCurrentIndex(model()->index(row, 0));

    // If the change didn't succeed (i.e. user selected "Cancel"), fail too
    if (groupIsHidden(currentIndex().row())) return true;
  }

  showHidden = show;

  if (showHidden) {
    for (int row = 0; row < model()->rowCount(); ++row) {
      displayGroup(row);
    }
  } else {
    for (int row = 0; row < model()->rowCount(); ++row) {
      if (groupIsHidden(row)) {
        hideGroup(row);
      }
    }
  }

  return showHidden;
}

void SettingsTopView::dataChanged(
  const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  QModelIndex parent = topLeft.parent();

  // If groups were modified, check whether they should be hidden
  if (!parent.isValid()) {
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
      // Hide hidden groups (if they should not be shown) or empty groups
      if ((!showHidden && groupIsHidden(row)) ||
          (groupIsEmpty(row))) {
        hideGroup(row);
      }
    }
  }

  QListView::dataChanged(topLeft, bottomRight);
}

void SettingsTopView::rowsInserted(
  const QModelIndex& parent, int start, int end) {
  // If the data belongs inside of a group, show the group (if allowed)
  if (parent.isValid()) {
    int row = parent.row();

    if (!groupIsEmpty(row)) {
      if (!groupIsHidden(row) || showHidden) {
        displayGroup(row);
      }
    }
  }
}

bool SettingsTopView::groupIsHidden(int row) {
  QModelIndex groupIndex = model()->index(row, 0);
  QVariant groupName = model()->data(groupIndex, Qt::UserRole);
  // The group name has "_hidden" at the end
  return hiddenGroupRegExp.indexIn(groupName.toString()) != -1;
}

bool SettingsTopView::groupIsEmpty(int row) {
  QModelIndex groupIndex = model()->index(row, 0);
  if (model()->rowCount(groupIndex) == 0)
    return true;
  else
    return false;
}

void SettingsTopView::hideGroup(int row) {
  setRowHidden(row, true);
}

void SettingsTopView::displayGroup(int row) {
  // Empty settings groups will stay hidden
  if (groupIsEmpty(row)) return;

  setRowHidden(row, false);
}

void SettingsTopView::currentChanged(
  const QModelIndex& current,
  const QModelIndex& previous) {
  if (groupChangeInProgress) return;
  if (!bottom) return;
  if (bottom->editorValuesChanged()) {
    QMessageBox::StandardButton butt =
      QMessageBox::question(NULL, tr("Unsaved changes in this tab."),
      tr("You have some unsaved changes to the setting values in the current "
      "settings group. Do you want to apply these changes or reset the "
      "settings to their previous values?"),
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

