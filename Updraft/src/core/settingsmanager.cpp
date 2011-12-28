#include "settingsmanager.h"

#include "updraft.h"
#include "ui/mainwindow.h"
#include "menu.h"
#include "../menuinterface.h"

namespace Updraft {
namespace Core {

SettingsManager::SettingsManager() {
  model = new QStandardItemModel(this);

  dialog = new SettingsDialog(NULL, model);

  // Create an action that shows the dialog and add it to the menu
  MainWindow* win = updraft->mainWindow;
  Menu* toolsMenu = win->getSystemMenu(MENU_TOOLS);

  settingsAction = new QAction(QIcon(":/core/icons/configure.png"), tr("Settings..."), this); // NOLINT
  settingsAction->setIconVisibleInMenu(true);
  QObject::connect(settingsAction, SIGNAL(triggered()), dialog, SLOT(exec()));
  toolsMenu->insertAction(100, settingsAction);

  // TODO(cestmir): Populate the test model with some testing data for now
  QStandardItem* firstRow = new QStandardItem();
  QStandardItem* setting1 = new QStandardItem();
  QStandardItem* setting2 = new QStandardItem();
  QStandardItem* setting3 = new QStandardItem();

  setting1->setData("String data", Qt::DisplayRole);
  firstRow->appendRow(setting1);

  firstRow->setData("Map settings", Qt::DisplayRole);
  firstRow->setIcon(QIcon(":/core/icons/configure.png"));
  model->appendRow(firstRow);
}

SettingsManager::~SettingsManager() {
  // TODO(cestmir): We probably need to destroy this, since it has no parent
  delete dialog;
}

}  // End namespace Core
}  // End namespace Updraft

