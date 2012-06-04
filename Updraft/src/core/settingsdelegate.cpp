#include "settingsdelegate.h"

#include <QMetaType>

#include "settingsmanager.h"
#include "basicsetting.h"
#include "coloreditor.h"
#include "directoryeditor.h"
#include "translationlanguageeditor.h"
#include "ellipsoidnameeditor.h"

namespace Updraft {
namespace Core {

SettingsDelegate::SettingsDelegate(QObject *parent)
  : QStyledItemDelegate(parent) {
  QItemEditorFactory *factory = new QItemEditorFactory;
  factory->registerEditor(QVariant::Color,
    new QStandardItemEditorCreator<ColorEditor>());

  setItemEditorFactory(factory);
}

QWidget* SettingsDelegate::createEditor(QWidget* parent,
  const QStyleOptionViewItem& option, const QModelIndex& index) const {
  QWidget* widget;
  QVariant data = index.model()->data(index, Qt::EditRole);
  int type = QMetaType::type(data.typeName());

  if (type == QMetaType::type("QDir")) {
    widget = new DirectoryEditor(parent);
  } else if (type == QMetaType::type("TranslationLanguage")) {
    widget = new TranslationLanguageEditor(parent);
  } else if (type == QMetaType::type("EllipsoidName")) {
    widget = new EllipsoidNameEditor(parent);
  } else {
    widget = QStyledItemDelegate::createEditor(parent, option, index);
  }
  return widget;
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

  QVariant name = model->data(index, Qt::UserRole);
  QVariant value = model->data(index, Qt::EditRole);

  // Only reset editor data if the property differs from the value in the model
  if (!variantsEqual(model->data(index, Qt::EditRole),
    editor->property(propertyName))) {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

}  // End namespace Core
}  // End namespace Updraft

