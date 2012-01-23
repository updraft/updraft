#include "settingsbottomview.h"

namespace Updraft {
namespace Core {

SettingsBottomView::SettingsBottomView(QWidget* parent) {
  // Override the default rendering style of QListView
  setFrameStyle(QFrame::Plain);
  QPalette pal = palette();
  pal.setColor(QPalette::Base, pal.color(QPalette::Window));
  setPalette(pal);

  stack = new QStackedWidget(this);

  createEditors();
}

SettingsBottomView::~SettingsBottomView() {}

QModelIndex SettingsBottomView::indexAt(const QPoint& point) const {
  if (!model())
    return QModelIndex();

  return QModelIndex();
}

void SettingsBottomView::scrollTo(const QModelIndex& index, ScrollHint hint) {
  return;
}

QRect SettingsBottomView::visualRect(const QModelIndex& index) const {
  return rect();
}

void SettingsBottomView::setModel(QAbstractItemModel* model) {
  QAbstractItemView::setModel(model);

  createEditors();
}

void SettingsBottomView::setTopIndex(const QModelIndex& index) {
  if (stack->currentIndex() != index.row()) {
    stack->setCurrentIndex(index.row());
  }

  return;
}

void SettingsBottomView::commit() {
  int pages = stack->count();

  for (int p = 0; p < pages; ++p) {
    QWidget* page = stack->widget(p);
    QGridLayout* layout = qobject_cast<QGridLayout*>(page->layout());
    QModelIndex pageIndex = model()->index(p, 0);

    for (int i = 0; i < layout->rowCount(); ++i) {
      QLayoutItem* item = layout->itemAtPosition(i, 1);
      QWidget* editor = item->widget();
      QModelIndex dataIndex = model()->index(i, 0, pageIndex);
      itemDelegate()->setModelData(editor, model(), dataIndex);
    }
  }
}

void SettingsBottomView::reset() {
  int pages = stack->count();

  for (int p = 0; p < pages; ++p) {
    QWidget* page = stack->widget(p);
    QGridLayout* layout = qobject_cast<QGridLayout*>(page->layout());
    QModelIndex pageIndex = model()->index(p, 0);

    for (int i = 0; i < layout->rowCount(); ++i) {
      QLayoutItem* item = layout->itemAtPosition(i, 1);
      QWidget* editor = item->widget();
      QModelIndex dataIndex = model()->index(i, 0, pageIndex);
      itemDelegate()->setEditorData(editor, dataIndex);
    }
  }
}

void SettingsBottomView::createEditors() {
  QModelIndex index;
  QGridLayout* layout;

  if (!model()) {
    return;
  }

  delete stack;
  stack = new QStackedWidget(this);

  for (int page = 0; page < model()->rowCount(); ++page) {
    layout = new QGridLayout();

    index = model()->index(page, 0);
    for (int row = 0; row < model()->rowCount(index); ++row) {
      QModelIndex child = model()->index(row, 0, index);

      QVariant descData = model()->data(child, Qt::DisplayRole);
      QLabel* label = new QLabel(descData.toString(), NULL);
      layout->addWidget(label, row, 0);

      QWidget* editor = createEditorForIndex(child);

      layout->addWidget(editor, row, 1);
      editor->show();
    }

    QWidget* page = new QWidget(NULL);
    page->setLayout(layout);
    stack->addWidget(page);
  }
}

QWidget* SettingsBottomView::createEditorForIndex(const QModelIndex& index) {
  QAbstractItemDelegate* delegate = itemDelegate(index);

  QStyleOptionViewItem sovi;
  sovi.displayAlignment = Qt::AlignRight;
  sovi.decorationPosition = QStyleOptionViewItem::Right;

  QWidget* editor = delegate->createEditor(NULL, sovi, index);
  delegate->setEditorData(editor, index);

  return editor;
}

void SettingsBottomView::paintEvent(QPaintEvent * event) {}

int SettingsBottomView::horizontalOffset() const {
  return 0;
}

bool SettingsBottomView::isIndexHidden(const QModelIndex & index) const {
  return false;
}

QModelIndex SettingsBottomView::moveCursor(
  CursorAction cursorAction,
  Qt::KeyboardModifiers modifiers) {
  return QModelIndex();
}

void SettingsBottomView::setSelection(
  const QRect & rect,
  QItemSelectionModel::SelectionFlags flags) {}

int SettingsBottomView::verticalOffset() const {
  return 5;
}

QRegion SettingsBottomView::visualRegionForSelection(
  const QItemSelection & selection) const {
  return QRegion();
}

void SettingsBottomView::commitData(QWidget* editor) {
}

void SettingsBottomView::dataChanged(
  const QModelIndex& topLeft,
  const QModelIndex& bottomRight) {
  // If modifying data for a group, don't do anything
  if (!insertionIndex.isValid()) return;

  // Don't allow modifying more than one data element
  if (topLeft != bottomRight) {
    qDebug() << "Warning: the settings view does not allow batch modification";
    return;
  }

  QModelIndex parent = topLeft.parent();

  QWidget* page = stack->widget(parent.row());
  QGridLayout* layout = qobject_cast<QGridLayout*>(page->layout());
  QWidget* widget;
  QLayoutItem* item;

  // Setting description
  item = layout->itemAtPosition(topLeft.row(), 0);
  if (!item) {
    widget = new QLabel(topLeft.data(Qt::DisplayRole).toString(), NULL);
    layout->addWidget(widget, topLeft.row(), 0);
  } else {
    widget = item->widget();
    qobject_cast<QLabel*>(widget)->setText(
      topLeft.data(Qt::DisplayRole).toString());
  }

  // Setting value
  item = layout->itemAtPosition(topLeft.row(), 1);
  if (!item) {
    widget = createEditorForIndex(topLeft);
    layout->addWidget(widget, topLeft.row(), 1);
    widget->show();
  } else {
    widget = item->widget();
  }
  QAbstractItemDelegate* delegate = itemDelegate(topLeft);
  delegate->setEditorData(widget, topLeft);
}

void SettingsBottomView::rowsInserted(
  const QModelIndex& parent, int start, int end) {
  QModelIndex index;
  QGridLayout* layout;
  QWidget* page;

  if (!parent.isValid()) {
    layout = new QGridLayout();
    page = new QWidget(NULL);
    page->setLayout(layout);
    stack->addWidget(page);
  }

  // Invalid insertion index will mark insertion of a group
  insertionIndex = parent;
}

}  // End namespace Core
}  // End namespace Updraft

