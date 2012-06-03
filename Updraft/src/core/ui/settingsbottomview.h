#ifndef UPDRAFT_SRC_CORE_UI_SETTINGSBOTTOMVIEW_H_
#define UPDRAFT_SRC_CORE_UI_SETTINGSBOTTOMVIEW_H_

#include <QtGui>

namespace Updraft {
namespace Core {

// TODO(cestmir): This needs heavy documenting

class SettingsBottomView: public QAbstractItemView {
  Q_OBJECT

 public:
  explicit SettingsBottomView(QWidget* parent);
  ~SettingsBottomView();

  QModelIndex indexAt(const QPoint& point) const;
  void scrollTo(const QModelIndex& index, ScrollHint hint = EnsureVisible);
  QRect visualRect(const QModelIndex& index) const;

  /// Tells whether there are any changes on the current page.
  bool editorValuesChanged();

  void setModel(QAbstractItemModel* model);

  void setTopIndex(const QModelIndex& index);

  void setItemDelegate(QAbstractItemDelegate *delegate);

 public slots:
  void commit();
  void reset();

 signals:
  void restartInfo(bool needsRestart);

 protected:
  void createEditors();
  QWidget* createEditorForIndex(const QModelIndex& index);

  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

  int horizontalOffset() const;
  bool isIndexHidden(const QModelIndex & index) const;
  QModelIndex moveCursor(CursorAction cursorAction,
    Qt::KeyboardModifiers modifiers);
  void setSelection(const QRect & rect,
    QItemSelectionModel::SelectionFlags flags);
  int verticalOffset() const;
  QRegion visualRegionForSelection(const QItemSelection & selection) const;

  bool someSettingNeedsRestart();

 protected slots:
  void commitData(QWidget* editor);
  void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
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

