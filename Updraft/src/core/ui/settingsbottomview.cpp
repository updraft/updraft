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

SettingsBottomView::~SettingsBottomView() {
  EditorList::iterator it = children.begin();
  while (it != children.end()) {
    delete (*it);
    ++it;
  }
}

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
    QList<QWidget*>* childrenPage = new QList<QWidget*>;

    index = model()->index(page, 0);
    for (int row = 0; row < model()->rowCount(index); ++row) {
      QModelIndex child     = model()->index(row, 1, index);
      QModelIndex childName = model()->index(row, 0, index);
      QAbstractItemDelegate* delegate = itemDelegate(child);

      QVariant nameData = model()->data(childName);
      QLabel* label = new QLabel(nameData.toString(), NULL);
      layout->addWidget(label, row, 0);

      QStyleOptionViewItem sovi;
      sovi.displayAlignment = Qt::AlignRight;
      sovi.decorationPosition = QStyleOptionViewItem::Right;

      QWidget* editor = delegate->createEditor(NULL, sovi, child);
      delegate->setEditorData(editor, child);
      layout->addWidget(editor, row, 1);
      childrenPage->push_back(editor);
      editor->show();

      if (!editor) qDebug() << "No delegate :-(";
      else qDebug() << "Delegate is of type " << editor->metaObject()->className();
    }

    children.push_back(childrenPage);

    QWidget* page = new QWidget(NULL);
    page->setLayout(layout);
    stack->addWidget(page);
  }
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

void SettingsBottomView::commitEditorData(QWidget* editor) const {
  // Find the model index of the editor
  /*int childRow = children.indexOf(editor);
  QModelIndex topIndex = model()->index(topRow, 0);
  QModelIndex childIndex = model()->index(childRow, 0, topIndex);*/

  // COmmit the data using the delegate
/*  QAbstractItemDelegate* delegate = itemDelegate(childIndex);
  delegate->setModelData(editor, model(), childIndex);*/
}

void SettingsBottomView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
  qDebug() << "Data changed";
}

void SettingsBottomView::rowsInserted(const QModelIndex& parent, int start, int end) {
  qDebug() << "Rows inserted";
}

}  // End namespace Core
}  // End namespace Updraft

