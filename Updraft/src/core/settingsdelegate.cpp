#include "settingsdelegate.h"
#include "settingsmanager.h"
#include "basicsetting.h"

#include "coloreditor.h"

namespace Updraft {
namespace Core {

SettingsDelegate::SettingsDelegate(QObject *parent)
  : QStyledItemDelegate(parent) {
  QItemEditorFactory *factory = new QItemEditorFactory;
  factory->registerEditor(QVariant::Color,
    new QStandardItemEditorCreator<ColorEditor>());

  setItemEditorFactory(factory);
}

void SettingsDelegate::setModelData(
  QWidget* editor,
  QAbstractItemModel* model,
  const QModelIndex& index) const {
  QByteArray propertyName = editor->metaObject()->userProperty().name();

  // Only set the value if the property differs from the value in the model
  if (!variantsEqual(model->data(index, Qt::EditRole),
    editor->property(propertyName))) {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

void SettingsDelegate::setEditorData(
  QWidget* editor,
  const QModelIndex& index) const {
  QByteArray propertyName = editor->metaObject()->userProperty().name();
  const QAbstractItemModel* model = index.model();

  // Only reset editor data if the property differs from the value in the model
  if (!variantsEqual(model->data(index, Qt::EditRole),
    editor->property(propertyName))) {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

}  // End namespace Core
}  // End namespace Updraft

