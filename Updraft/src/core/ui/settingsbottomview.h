#ifndef UPDRAFT_SRC_CORE_UI_SETTINGSBOTTOMVIEW_H_
#define UPDRAFT_SRC_CORE_UI_SETTINGSBOTTOMVIEW_H_

#include <QtGui>

namespace Updraft {
namespace Core {

// TODO(cestmir): This needs heavy documenting

/// List showing the individual setting items within one group.
/// The setting items are the first items in each row under the given group
/// model index. All the data of the setting is saved in the same data item
/// under various data roles. The view creates all the setting labels and
/// editors at the time of its creation and then switches between different
/// setting group pages in a QStackedWidget.
class SettingsBottomView: public QAbstractItemView {
  Q_OBJECT

 public:
  explicit SettingsBottomView(QWidget* parent);
  ~SettingsBottomView();

  /// Gets the index of the item at the given point.
  /// Currently not used and unimplemented.
  /// \param point Not used.
  /// \return Invalid model index.
  QModelIndex indexAt(const QPoint& point) const;

  /// Scrolls to the given model index.
  /// Currently not used and unimplemented.
  /// \param index Not used.
  /// \param hint Not used.
  void scrollTo(const QModelIndex& index, ScrollHint hint = EnsureVisible);

  /// The visual rectangle that encapsulates the given model index.
  /// Currently not used and unimplemented.
  /// \param index Not used.
  /// \return Empty rectangle.
  QRect visualRect(const QModelIndex& index) const;

  /// Tells whether there are any changes on the current page.
  /// \return Whether the values on the current page have changed.
  bool editorValuesChanged();

  /// Sets the data model used by this view.
  /// \param model The data model used by this view.
  void setModel(QAbstractItemModel* model);

  /// Reacts to the top view selected group index change.
  /// It changes the index of the page of the stack widget that is being
  /// displayed.
  /// \param index Model index of the newly selected group.
  void setTopIndex(const QModelIndex& index);

  /// Sets the item delegate used by this view.
  void setItemDelegate(QAbstractItemDelegate *delegate);

 public slots:
  /// Commits the data from editors into the model.
  void commit();
  /// Resets the values in the editors to the values that are in the model.
  void reset();

 signals:
  /// Signals whether there is a setting in the currently displayed page that
  /// needs the application to restart on its value change.
  /// \param needsRestart Whether there is a setting that needs the restart.
  void restartInfo(bool needsRestart);

 protected:
  /// Creates all the setting labels and editors. The editors are created using
  /// the previously given delegate, which should be the SettingsDelegate
  /// instance. If the editors already exist, they are deleted and created
  /// again.
  void createEditors();

  /// Helper method that creates an editor for the item at a given index.
  /// The item's editor is created using the previously supplied delegate.
  /// \param index Model index of the item whose editor should be created.
  /// \return Pointer to the newly created editor.
  QWidget* createEditorForIndex(const QModelIndex& index);

  /// Reacts to the Qt repaint event.
  /// \param event The event information.
  void paintEvent(QPaintEvent* event);

  /// Reacts to the Qt resize event.
  /// \param event The event information.
  void resizeEvent(QResizeEvent* event);

  /// Currently unimplemented, but defined to be able to instantiate objects.
  /// \return 0
  int horizontalOffset() const;

  /// Currently unimplemented, but defined to be able to instantiate objects.
  /// \return false
  bool isIndexHidden(const QModelIndex & index) const;

  /// Returns the description of the setting at the given index.
  /// \param The model index of the setting whose description we want.
  /// \return String value that was set as the description for the given
  ///         setting.
  QString getSettingDescription(const QModelIndex& settingIndex) const;

  /// Returns whether the given setting requires the application to restart
  /// if its value changes.
  /// \param settingIndex The model index of the setting
  /// \return Whether the setting at the given index reqires a restart on
  ///         value change.
  bool settingNeedsRestart(const QModelIndex& settingIndex) const;

  /// Reacts to the cursor movement over the view.
  /// Currently unimplemented, but defined to be able to instantiate objects.
  /// \param cursorAction Not used
  /// \param modifiers Not used
  /// \return Invalid model index
  QModelIndex moveCursor(CursorAction cursorAction,
    Qt::KeyboardModifiers modifiers);

  /// Sets the selection in this view.
  /// Currently unimplemented, but defined to be able to instantiate objects.
  /// \param rect Not used
  /// \param flags Not used
  void setSelection(const QRect & rect,
    QItemSelectionModel::SelectionFlags flags);

  /// Gets vertical offset between the individual items.
  /// \return 5
  int verticalOffset() const;

  /// Returns a visual region corresponding to the given set of selected items.
  /// Currently unimplemented, but defined to be able to instantiate objects.
  /// \param selection Not used
  /// \return Invalid QRegion
  QRegion visualRegionForSelection(const QItemSelection & selection) const;

  /// Finds out whether there is a setting in the currently displayed group
  /// that needs the application to restart on its value change.
  /// \return Whether there is such setting in the current group.
  bool someSettingNeedsRestart();

 protected slots:
  /// Commits the data of the given editor.
  /// Not used. A different mechanism is used for data commiting. See methods
  /// commit() and reset().
  /// \param editor Not used
  void commitData(QWidget* editor);

  /// Reacts to the change of the data of a (set of) setting(s).
  /// Currently supports only data changes of single settings. Therefore,
  /// the topLeft index must always equal to the bottomRight index.
  /// \param topLeft The top-left index of the set of items whose data has
  ///                changed.
  /// \param bottomRight The bottom-right index of the set of items whose data
  ///                    has changed.
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

  /// Reacts to a row insertion on the underlying model.
  /// \param parent The model index of the parent item, under which the new
  ///               rows were inserted.
  /// \param start The first index of the newly inserted rows.
  /// \param end Index one-after the last inserted row.
  void rowsInserted(const QModelIndex& parent, int start, int end);

 private:
  /// Stack of the top-level widgets
  QStackedWidget* stack;

  /// A helper index used when reacting to a setting data row insertion
  QModelIndex insertionIndex;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_SETTINGSBOTTOMVIEW_H_

