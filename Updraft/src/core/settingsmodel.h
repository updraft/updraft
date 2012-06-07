#ifndef UPDRAFT_SRC_CORE_SETTINGSMODEL_H_
#define UPDRAFT_SRC_CORE_SETTINGSMODEL_H_

#include <QtGui>
#include <QtXml/QDomDocument>

namespace Updraft {
namespace Core {

class SettingsModel;

/// A single setting. It contains a QDomNode that points to the item in the
/// QDomDocument hierarchy that contains the setting's data.
class SettingsItem {
 public:
  /// Creates the settings item to contain the given QDomNode, belonging to
  /// the given SettingsModel.
  /// \param node The QDomNode that this SettingsItem should contain.
  /// \param model The data model to which this setting belongs.
  SettingsItem(QDomNode node, SettingsModel* model);
  ~SettingsItem();

  /// Adds a child SettingsItem before all children in this SettingsItem.
  /// \param item The item to prepend as a child of this item.
  void prependChild(SettingsItem* item);

  /// Adds a child SettingsItem after all children in this SettingsItem.
  /// \param item The item to append as a child of this item.
  void appendChild(SettingsItem* item);

  /// Adds a child SettingsItem into the given position in the children list
  /// of this item.
  /// \param item The item to insert as a child of this item.
  /// \param index The index at which the item should be inserted.
  void insertChild(SettingsItem* item, int index);

  /// Gets the child SettingsItem at the given index.
  /// \return The desired child item.
  SettingsItem* getChild(int index);

  /// Returns the index of a child SettingsItem in this SettingsItem.
  /// \param item The child whose index is to be retrieved.
  /// \return Index of the child item.
  int childIndex(SettingsItem* item);

  /// \return The parent item of this SettingsItem. Can be NULL if this
  /// item has no parent.
  SettingsItem* getParent() { return parent; }

  /// \return A copy of the QDomNode that points to the setting data in the
  /// SettingsModel.
  QDomNode getNode() { return domNode; }

  /// \return Number of rows in this item - i.e. the number of child items.
  int rowCount();

  /// \return The SettingsModel for this item.
  SettingsModel* getModel() { return myModel; }

  /// Sets the icon for this settings item. Used for setting group icons.
  /// \param i The new icon for the item.
  void setIcon(QIcon i) { icon = i; }

  /// \return The currently used icon for the item. Can be an empty icon if no
  ///         icon was set for this item.
  QIcon getIcon() { return icon; }

 private:
  /// The QDomNode pointing to the data of this setting.
  QDomNode domNode;

  /// Children of this SettingsItem SettingsModel hierarchy.
  QVector<SettingsItem*> children;

  /// Parent of this SettingsItem in the SettingsModel hierarchy.
  SettingsItem* parent;

  /// The SettingsModel where the data for this item is stored.
  SettingsModel* myModel;

  /// Icon used by this item.
  QIcon icon;
};

/// Custom names for data roles in settings.
enum SettingsDataRole {
    ValueRole = Qt::EditRole,
    DecorationRole = Qt::DecorationRole,
    DescriptionRole = Qt::DisplayRole,
    NameRole = Qt::UserRole,
    DefaultValueRole = Qt::UserRole+1,
    NeedsRestartRole = Qt::UserRole+2
};

/// Model that stores all settings.
/// The setting values are internally stored in a QDomDocument and a hierarchy
/// of SettingsItem instances is kept that points to the QDomDocument.
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


  /// Returns the number of columns under the given index. This is always equal
  /// to 1 in the SettingsModel.
  /// \param The index whose number of columns should be retrieved
  /// \return Number of columns for the given index.
  int columnCount(const QModelIndex& index) const { return 1; }

  /// Gets the data for the given index and data role.
  /// \param index The model index denoting the desired setting or settings
  ///              group.
  /// \param role The data role for the data to be retrieved.
  /// \return The desired data.
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  /// Gets the index for the desired SettingsItem.
  /// \param row The row index of the desired item.
  /// \param column The column index of the desired item.
  /// \param parent The parent of the desired item.
  /// \return The child of the given parent index at the position [row, column]
  QModelIndex index(int row, int column,
    const QModelIndex& parent = QModelIndex()) const;

  /// Looks up the model index for a given SettingsItem.
  /// \param item The item whose QModelIndex should be returned.
  /// \return The model index for the given item.
  QModelIndex indexFromItem(SettingsItem* item) const;

  /// Inserts a row into the model under the given parent item.
  /// \param row The row index of the row to be inserted.
  /// \param parent The model index of the parent item.
  /// \return A boolean value indicating success or failure.
  bool insertRow(int row, const QModelIndex& parent = QModelIndex());

  /// Converts the model index to the item at that index.
  /// \param index The model index of the item to be retrieved.
  /// \return Pointer to the SettingsItem at the given index.
  SettingsItem* itemFromIndex(const QModelIndex& index) const;

  /// Returns a parent model index for the given index.
  /// \param index The model index whose parent we want.
  /// \return The parent index for the given model index.
  QModelIndex parent(const QModelIndex& index) const;

  /// Returns the number of rows under the given index.
  /// \parent The index whose number of rows we want.
  /// \return Number of rows for the given index.
  int rowCount(const QModelIndex& parent = QModelIndex()) const;

  /// Sets the item data.
  /// \param index The index of the item whose data we want to set.
  /// \param value The value to be set.
  /// \param role The desired data role of the data being set.
  /// \return A boolean value indicating success or failure.
  bool setData(const QModelIndex& index, const QVariant& value,
    int role = Qt::DisplayRole);

  /// Gets the index of a sibling item to the given item.
  /// \param row Row index of the sibling.
  /// \param column Column index of the sibling.
  /// \param index The model index of the item whose sibling we want.
  /// \return The desired sibling item's index.
  QModelIndex sibling(int row, int column, const QModelIndex& index) const;

 signals:
  /// Signals that an item's data changed
  /// \param item The item whose data was changed
  void itemChanged(SettingsItem* item);

 private:
  /// Gets a textual representation of the given data role.
  /// This representation is used when saving into the settings file.
  /// \param role The data role whose textual representation we want.
  /// \return The textual representation of the given data role.
  QString dataRoleToString(int role) const;

  /// Internal helper function for adding rows.
  /// \param row The row index of the new row.
  /// \param item The SettingsItem under which we want the row to be added.
  void insertRowInternal(int row, SettingsItem* item);

  /// Reinitializes the internal data structures, effectively erasing the
  /// whole data model.
  void reinitializeData();

  /// The representation of the XML document where the settings are stored.
  QDomDocument domDoc;

  /// The root item of the SettingsItem hieararchy.
  SettingsItem* rootItem;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SETTINGSMODEL_H_

