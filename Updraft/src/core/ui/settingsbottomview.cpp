#include "settingsbottomview.h"
#include "settingsmodel.h"
#include "variantfunctions.h"

namespace Updraft {
namespace Core {

SettingsBottomView::SettingsBottomView(QWidget* parent) {
  // Override the default rendering style of QListView
  setFrameStyle(QFrame::Plain);
  QPalette pal = palette();
  pal.setColor(QPalette::Base, pal.color(QPalette::Window));
  setPalette(pal);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);

  stack = new QStackedWidget();
  layout->addWidget(stack);

  this->setLayout(layout);

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

bool SettingsBottomView::editorValuesChanged() {
  QScrollArea* scrollArea = qobject_cast<QScrollArea*>(stack->currentWidget());
  QWidget* page = scrollArea->widget();
  QGridLayout* layout = qobject_cast<QGridLayout*>(page->layout());
  QModelIndex pageIndex = model()->index(stack->currentIndex(), 0);

  for (int i = 0; i < layout->rowCount(); ++i) {
    QLayoutItem* item = layout->itemAtPosition(i, 1);
    if (!item) continue;  // Prevent segfault for empty layout
    QWidget* editor = item->widget();
    QByteArray propertyName = editor->metaObject()->userProperty().name();
    QVariant editorData = editor->property(propertyName);

    QModelIndex dataIndex = model()->index(i, 0, pageIndex);
    QVariant modelData = model()->data(dataIndex, Qt::EditRole);

    if (!variantsEqual(editorData, modelData)) return true;
  }

  return false;
}

void SettingsBottomView::setModel(QAbstractItemModel* model) {
  QAbstractItemView::setModel(model);

  createEditors();
}

void SettingsBottomView::setTopIndex(const QModelIndex& index) {
  if (stack->currentIndex() != index.row()) {
    stack->setCurrentIndex(index.row());
  }

  // Emit signals that say whether there is a setting that needs a restart
  if (someSettingNeedsRestart()) {
    emit restartInfo(true);
  } else {
    emit restartInfo(false);
  }

  return;
}

void SettingsBottomView::commit() {
  int pages = stack->count();

  for (int p = 0; p < pages; ++p) {
    QScrollArea* scrollArea = qobject_cast<QScrollArea*>(stack->widget(p));
    QWidget* page = scrollArea->widget();
    QGridLayout* layout = qobject_cast<QGridLayout*>(page->layout());
    QModelIndex pageIndex = model()->index(p, 0);

    for (int i = 0; i < layout->rowCount(); ++i) {
      QLayoutItem* item = layout->itemAtPosition(i, 1);
      if (!item) continue;  // Prevent segfault for empty layout
      QWidget* editor = item->widget();
      QModelIndex dataIndex = model()->index(i, 0, pageIndex);
      itemDelegate()->setModelData(editor, model(), dataIndex);
    }
  }
}

void SettingsBottomView::reset() {
  int pages = stack->count();

  for (int p = 0; p < pages; ++p) {
    QScrollArea* scrollArea = qobject_cast<QScrollArea*>(stack->widget(p));
    QWidget* page = scrollArea->widget();
    QGridLayout* layout = qobject_cast<QGridLayout*>(page->layout());
    QModelIndex pageIndex = model()->index(p, 0);

    for (int i = 0; i < layout->rowCount(); ++i) {
      QLayoutItem* item = layout->itemAtPosition(i, 1);
      if (!item) continue;  // Prevent segfault for empty layout
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

    // Create labels and editors
    index = model()->index(page, 0);
    int row;
    for (row = 0; row < model()->rowCount(index); ++row) {
      QModelIndex child = model()->index(row, 0, index);
      QString description = getSettingDescription(child);

      QLabel* label = new QLabel(description, NULL);
      label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
      layout->addWidget(label, row, 0);

      QWidget* editor = createEditorForIndex(child);
      editor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
      layout->addWidget(editor, row, 1);

      editor->show();
    }

    // Adding a stretcher at the bottom of all settings
    QWidget* dummy = new QWidget();
    layout->addWidget(dummy, row, 0);
    layout->setRowStretch(row, 1);

    // Setting space on the page
    layout->setHorizontalSpacing(5);
    layout->setVerticalSpacing(5);
    layout->setContentsMargins(5, 5, 5, 5);

    QScrollArea* scrollArea = new QScrollArea();

    QWidget* page = new QWidget();
    page->setLayout(layout);
    page->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    page->setMinimumSize(0, 0);

    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setWidget(page);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    stack->addWidget(scrollArea);
  }

  this->layout()->addWidget(stack);
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
  int pages = stack->count();

  for (int p = 0; p < pages; ++p) {
    QScrollArea* scrollArea = qobject_cast<QScrollArea*>(stack->widget(p));
    QWidget* page = scrollArea->widget();
    page->setFixedWidth(scrollArea->width());
  }
}

void SettingsBottomView::resizeEvent(QResizeEvent* event) {
  int pages = stack->count();

  for (int p = 0; p < pages; ++p) {
    QScrollArea* scrollArea = qobject_cast<QScrollArea*>(stack->widget(p));
    QWidget* page = scrollArea->widget();
    page->setFixedWidth(scrollArea->width());
  }
}

int SettingsBottomView::horizontalOffset() const {
  return 0;
}

bool SettingsBottomView::isIndexHidden(const QModelIndex & index) const {
  return false;
}

QString SettingsBottomView::getSettingDescription(
  const QModelIndex& settingIndex) const {
  // Get the setting description and append "* " if it needs restart
  QString description =
    model()->data(settingIndex, DescriptionRole).toString();
  if (settingNeedsRestart(settingIndex)) {
    description = QString("* ") + description;
  }

  return description;
}

bool SettingsBottomView::settingNeedsRestart(
  const QModelIndex& settingIndex) const {
  QVariant needsRestart = model()->data(settingIndex, NeedsRestartRole);

  bool settingNeedsRestart = false;
  if (needsRestart.isValid() && needsRestart.toBool()) {
    settingNeedsRestart = true;
  }

  return settingNeedsRestart;
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

bool SettingsBottomView::someSettingNeedsRestart() {
  QModelIndex pageIndex = model()->index(stack->currentIndex(), 0);
  if (!pageIndex.isValid()) return false;

  for (int row = 0; row < model()->rowCount(pageIndex); ++row) {
    QModelIndex settingIndex = model()->index(row, 0, pageIndex);
    if (settingNeedsRestart(settingIndex)) return true;
  }

  return false;
}

void SettingsBottomView::commitData(QWidget* editor) {
}

void SettingsBottomView::dataChanged(
  const QModelIndex& topLeft,
  const QModelIndex& bottomRight) {
  QModelIndex parent = topLeft.parent();

  // If modifying data for a group, don't do anything
  if (!parent.isValid()) return;

  // Don't allow modifying more than one data element
  if (topLeft != bottomRight) {
    qDebug() << "Warning: the settings view does not allow batch modification";
    return;
  }

  QScrollArea* scrollArea =
    qobject_cast<QScrollArea*>(stack->widget(parent.row()));
  QWidget* page = scrollArea->widget();
  QGridLayout* layout = qobject_cast<QGridLayout*>(page->layout());
  QWidget* widget;
  QLayoutItem* item;

  // Setting description
  item = layout->itemAtPosition(topLeft.row(), 0);
  QString description = getSettingDescription(topLeft);
  if (!item) {
    widget = new QLabel(description, NULL);
    layout->addWidget(widget, topLeft.row(), 0);
  } else {
    widget = item->widget();
    qobject_cast<QLabel*>(widget)->setText(description);
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

    layout->setHorizontalSpacing(5);
    layout->setVerticalSpacing(5);
    layout->setContentsMargins(5, 5, 5, 5);

    QScrollArea* scrollArea = new QScrollArea();

    page = new QWidget();
    page->setLayout(layout);
    page->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    page->setMinimumSize(0, 0);

    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setWidget(page);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    stack->addWidget(scrollArea);
  }

  // Invalid insertion index will mark insertion of a group
  insertionIndex = parent;
}

void SettingsBottomView::setItemDelegate(QAbstractItemDelegate *delegate) {
  QAbstractItemView::setItemDelegate(delegate);
  createEditors();
}

}  // End namespace Core
}  // End namespace Updraft

