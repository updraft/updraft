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

  // TODO(cestmir): Are old editors destroyed?
  createEditors();
}

void SettingsBottomView::setTopIndex(const QModelIndex& index) {
  qDebug() << "Bottom View changed. New index: " << index.row() << ":" << index.column();

  if (stack->currentIndex() != index.row()) {
    stack->setCurrentIndex(index.row());
  }

  return;
}

void SettingsBottomView::createEditors() {
  QModelIndex index;
  QGridLayout* layout;

  // TODO(cestmir): Remove debugging messages
  if (!model()) {
    qDebug() << "Model is null";
    return;
  } else {
    qDebug() << "Model is NOT null";
  }

  for (int page = 0; page < model()->rowCount(); ++page) {
    layout = new QGridLayout();

    index = model()->index(page, 1);
    for (int row = 0; row < model()->rowCount(index); ++row) {
      QModelIndex child     = model()->index(row, 2, index);
      QModelIndex childDesc = model()->index(row, 1, index);
      QModelIndex childName = model()->index(row, 0, index);

      qDebug() << "Child is " << child;

      QVariant descData = model()->data(childDesc);
      QLabel* label = new QLabel(descData.toString(), NULL);
      layout->addWidget(label, row, 0);

      QWidget* editor = createEditorForIndex(child);

      layout->addWidget(editor, row, 1);
      editor->show();

      if (!editor) qDebug() << "No delegate :-(";
      else qDebug() << "Delegate is of type " << editor->metaObject()->className();
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

void SettingsBottomView::paintEvent(QPaintEvent * event) {
  qDebug() << "Painting...";
}

int SettingsBottomView::horizontalOffset() const {

}

bool SettingsBottomView::isIndexHidden(const QModelIndex & index) const {

}

QModelIndex SettingsBottomView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) {

}

void SettingsBottomView::setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags flags) {

}

int SettingsBottomView::verticalOffset() const {
  return 5;
}

QRegion SettingsBottomView::visualRegionForSelection(const QItemSelection & selection) const {

}

void SettingsBottomView::commitData(QWidget* editor) {
  qDebug() << "Commit data";
}

void SettingsBottomView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  qDebug() << "Data changed: " << topLeft << " - " << bottomRight;

  // If modifying data for a group, don't do anything
  if (!insertionIndex.isValid()) return;

  // Don't allow modifying more than one data element
  if (topLeft != bottomRight) {
    qDebug() << "Warning: the settings model does not allow batch modification";
    return;
  }

  QModelIndex parent = topLeft.parent();

  QWidget* page = stack->widget(parent.row());
  QGridLayout* layout = qobject_cast<QGridLayout*>(page->layout());
  QWidget* widget;
  QLayoutItem* item;
  switch (topLeft.column()) {
    case 1:  // Setting description
      item = layout->itemAtPosition(topLeft.row(), 0);
      if (!item) {
        widget = new QLabel(topLeft.data().toString(), NULL);
        layout->addWidget(widget, topLeft.row(), 0);
      } else {
        widget = item->widget();
        qobject_cast<QLabel*>(widget)->setText(topLeft.data().toString());
      }
    break;
    case 2:  // Setting value
      item = layout->itemAtPosition(topLeft.row(), 1);
      if (item) {
        widget = item->widget();
        delete widget;
      }
      widget = createEditorForIndex(topLeft);
      layout->addWidget(widget, topLeft.row(), 1);
      widget->show();
    break;
  }
}

void SettingsBottomView::rowsInserted(const QModelIndex& parent, int start, int end) {
  qDebug() << "Rows inserted: " << parent;

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

