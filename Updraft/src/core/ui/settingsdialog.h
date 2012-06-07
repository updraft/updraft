#ifndef UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_
#define UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_

#include <QtGui>
#include "settingstopview.h"

namespace Ui { class SettingsDialog; }

namespace Updraft {
namespace Core {

class SettingsDelegate;
class SettingsManager;

/// Dialog window for editing settings.
/// Contains two views - SettingsTopView displays setting groups and
/// SettingsBottomView displays the individual settings for a group.
class SettingsDialog: public QDialog {
  Q_OBJECT

 public:
  /// Initializes the dialog, setting its parent and most importantly the
  /// SettingsManager.
  /// \param parent The QWidget parent of the dialog widget.
  /// \param manager The SettingsManager from which the dialog takes the data
  SettingsDialog(QWidget* parent, SettingsManager* manager);
  ~SettingsDialog();

  /// Sets the data model for the dialog. The dialog makes no assumptions about
  /// the specific class of the model, so even the standard Qt's data models
  /// can be used.
  /// \param model The model to be used by this dialog
  void setModel(QAbstractItemModel* model);

  /// Updates the width of the top view widget so that it accomodates all the
  /// group names.
  void recalculateTopViewWidth();

  /// Commits the data of the bottom view editors to the model.
  void commitEditorData();

  /// Reverts data of the bottom view editors to values present in the model.
  void resetEditors();

 protected slots:
  /// Handles the click events on all buttons of the dialog.
  /// \param button Pointer to the button that emited the event.
  void buttonBoxClicked(QAbstractButton* button);

  /// Handles the toggle of the advanced setting group toggling check-box.
  void hideCheckboxToggled();

  /// Shows/hides the label that explains the asterisk in the setting names.
  void restartNeeded(bool needed);

 protected:
  /// The pointer to the UI of this dialog
  Ui::SettingsDialog* ui;

  /// Delegate used to create the editors for all settings
  SettingsDelegate* settingsDelegate;

  /// The manager that was used to create this dialog.
  SettingsManager* settingsManager;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_SETTINGSDIALOG_H_

