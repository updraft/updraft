#include "settingsmanager.h"

#include "updraft.h"
#include "ui/mainwindow.h"
#include "menu.h"
#include "../menuinterface.h"

namespace Updraft {
namespace Core {

SettingsManager::SettingsManager() {
  model = new QStandardItemModel(this);

  // TODO(cestmir): Populate the test model with some testing data for now
  QStandardItem* firstRow = new QStandardItem();
  QStandardItem* secondRow = new QStandardItem();
  QStandardItem* item;

  int row = 0;

  item = new QStandardItem();
  item->setData("String data", Qt::DisplayRole);
  firstRow->setChild(row, 0, item);
  item = new QStandardItem();
  item->setData("The data", Qt::DisplayRole);
  firstRow->setChild(row, 1, item);
  row++;

  item = new QStandardItem();
  item->setData("Value of PI", Qt::DisplayRole);
  firstRow->setChild(row, 0, item);
  item = new QStandardItem();
  item->setData(3.14, Qt::DisplayRole);
  firstRow->setChild(row, 1, item);
  row++;

  item = new QStandardItem();
  item->setData("Red color", Qt::DisplayRole);
  firstRow->setChild(row, 0, item);
  item = new QStandardItem();
  item->setData(QColor(255, 0, 0), Qt::DisplayRole);
  firstRow->setChild(row, 1, item);
  row++;

  row = 0;

  item = new QStandardItem();
  item->setData("The answer to life, universe and everything", Qt::DisplayRole);
  secondRow->setChild(row, 0, item);
  item = new QStandardItem();
  item->setData(42, Qt::DisplayRole);
  secondRow->setChild(row, 1, item);
  row++;

  firstRow->setData("Map settings", Qt::DisplayRole);
  firstRow->setIcon(QIcon(":/core/icons/configure.png"));
  model->appendRow(firstRow);

  secondRow->setData("Other settings", Qt::DisplayRole);
  secondRow->setIcon(QIcon(":/core/icons/configure.png"));
  model->appendRow(secondRow);

  // Create the dialog
  dialog = new SettingsDialog(NULL, model);

  // Create an action that shows the dialog and add it to the menu
  MainWindow* win = updraft->mainWindow;
  Menu* toolsMenu = win->getSystemMenu(MENU_TOOLS);

  settingsAction = new QAction(QIcon(":/core/icons/configure.png"), tr("Settings..."), this); // NOLINT
  settingsAction->setIconVisibleInMenu(true);
  QObject::connect(settingsAction, SIGNAL(triggered()), dialog, SLOT(exec()));
  toolsMenu->insertAction(100, settingsAction);
}

SettingsManager::~SettingsManager() {
  // TODO(cestmir): We probably need to destroy this, since it has no parent
  delete dialog;
}

}  // End namespace Core
}  // End namespace Updraft

