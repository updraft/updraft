#include "settingstopview.h"
#include "settingsbottomview.h"

namespace Updraft {
namespace Core {

SettingsTopView::SettingsTopView(QWidget* parent): QListView(parent) {
  bottom = NULL;
  advancedGroupRegExp = QRegExp("_advanced$");
  hiddenGroupRegExp = QRegExp("_hidden$");
  showAdvanced = false;
  groupChangeInProgress = false;
  setSelectionMode(QAbstractItemView::SingleSelection);
}

void SettingsTopView::setModel(QAbstractItemModel* model) {
  QAbstractItemView::setModel(model);

  for (int row = 0; row < model->rowCount(); ++row) {
    if (groupShouldBeHidden(row)) {
      hideGroup(row);
    }
  }
}

bool SettingsTopView::setShowAdvanced(bool show) {
  // If the currently selected group should be hidden, try to change it
  if (currentIndex().isValid() &&
    showAdvanced &&
    !show &&
    groupIsAdvanced(currentIndex().row())) {
    // Find the first row that doesn't have to be hidden and try to select it
    int row;
    for (row = 0; row < model()->rowCount(); ++row) {
      if (!groupIsAdvanced(row)) break;
    }
    setCurrentIndex(model()->index(row, 0));

    // If the change didn't succeed (i.e. user selected "Cancel"), fail too
    if (groupIsAdvanced(currentIndex().row())) return true;
  }

  showAdvanced = show;

  if (showAdvanced) {
    for (int row = 0; row < model()->rowCount(); ++row) {
      displayGroup(row);
    }
  } else {
    for (int row = 0; row < model()->rowCount(); ++row) {
      if (groupIsAdvanced(row)) {
        hideGroup(row);
      }
    }
  }

  return showAdvanced;
}

void SettingsTopView::dataChanged(
  const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  QModelIndex parent = topLeft.parent();

  // If groups were modified, check whether they should be hidden
  if (!parent.isValid()) {
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
      // Hide hidden groups (if they should not be shown) or empty groups
      if ((!showAdvanced && groupIsAdvanced(row)) ||
          (groupShouldBeHidden(row))) {
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

    if (!groupShouldBeHidden(row)) {
      if (!groupIsAdvanced(row) || showAdvanced) {
        displayGroup(row);
      }
    }
  }
}

bool SettingsTopView::groupIsAdvanced(int row) {
  QModelIndex groupIndex = model()->index(row, 0);
  QVariant groupName = model()->data(groupIndex, Qt::UserRole);
  // The group name has "_advanced" at the end
  return advancedGroupRegExp.indexIn(groupName.toString()) != -1;
}

bool SettingsTopView::groupShouldBeHidden(int row) {
  QModelIndex groupIndex = model()->index(row, 0);
  if (model()->rowCount(groupIndex) == 0)
    return true;

  QVariant groupName = model()->data(groupIndex, Qt::UserRole);
  // The group name has "_hidden" at the end
  return hiddenGroupRegExp.indexIn(groupName.toString()) != -1;
}

void SettingsTopView::hideGroup(int row) {
  setRowHidden(row, true);
}

void SettingsTopView::displayGroup(int row) {
  // Some settings groups will stay hidden
  if (groupShouldBeHidden(row)) return;

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

