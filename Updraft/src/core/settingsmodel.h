#ifndef UPDRAFT_SRC_CORE_SETTINGSMODEL_H_
#define UPDRAFT_SRC_CORE_SETTINGSMODEL_H_

#include <QtGui>
#include <QtXml/QDomDocument>

namespace Updraft {
namespace Core {

class SettingsModel;

class SettingsItem {
 public:
  SettingsItem(QDomNode node, SettingsModel* model);
  ~SettingsItem();

  void prependChild(SettingsItem* item);
  void appendChild(SettingsItem* item);
  void insertChild(SettingsItem* item, int index);
  SettingsItem* getChild(int index);
  int childIndex(SettingsItem* item);
  SettingsItem* getParent() { return parent; }
  QDomNode getNode() { return domNode; }
  int rowCount();
  SettingsModel* getModel() { return myModel; }

  void setIcon(QIcon i) { icon = i; }
  QIcon getIcon() { return icon; }

 private:
  QDomNode domNode;
  QVector<SettingsItem*> children;
  SettingsItem* parent;
  SettingsModel* myModel;

  QIcon icon;
};

class SettingsModel: public QAbstractItemModel {
  Q_OBJECT

 public:
  SettingsModel();
  ~SettingsModel();

  /// Loads settings from a xml settings file.
  /// If the file does not exist, the application attempts to create it.
  /// \param filename Path to the settings file.
  void loadSettings(QString filename);

  /// Saves the settings to a settings file.
  /// \param filename Path to the settings file.
  void saveSettings(QString filename);

  int columnCount(const QModelIndex&) const { return 1; }
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column,
    const QModelIndex& parent = QModelIndex()) const;
  QModelIndex indexFromItem(SettingsItem* item) const;
  bool insertRow(int row, const QModelIndex& parent = QModelIndex());
  SettingsItem* itemFromIndex(const QModelIndex& index) const;
  QModelIndex parent(const QModelIndex&) const;
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  bool setData(const QModelIndex& index, const QVariant& value,
    int role = Qt::DisplayRole);
  QModelIndex sibling(int row, int column, const QModelIndex& index) const;

 signals:
  /// Signals that an item's data changed
  /// \param item The item whose data was changed
  void itemChanged(SettingsItem* item);

 private:
  QString dataRoleToString(int role) const;
  void insertRowInternal(int row, SettingsItem* item);

  void reinitializeData();

  QDomDocument domDoc;
  SettingsItem* rootItem;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSMODEL_H_

