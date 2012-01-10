#include "settingsdelegate.h"
#include "basicsetting.h"

namespace Updraft {
namespace Core {

void SettingsDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
  QByteArray propertyName = editor->metaObject()->userProperty().name();

  // Only set the value if the property differs from the value in the model
  if (!variantsEqual(model->data(index, Qt::EditRole), editor->property(propertyName))) {
    QStyledItemDelegate::setModelData(editor, model, index);

    // If the data was changed, let all the settings know about this
    for (int inst = 0; inst < model->rowCount(index); ++inst) {
      QModelIndex instanceIndex = model->index(inst, 0, index);
      QVariant instance = model->data(instanceIndex, Qt::EditRole);
      BasicSetting* instancePtr = (BasicSetting*)instance.value<void*>();
      instancePtr->emitValueChanged();
    }
  }
}

void SettingsDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
  QByteArray propertyName = editor->metaObject()->userProperty().name();
  const QAbstractItemModel* model = index.model();

  // Only reset editor data if the property differs from the value in the model
  if (!variantsEqual(model->data(index, Qt::EditRole), editor->property(propertyName))) {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

bool SettingsDelegate::variantsEqual(const QVariant& var1, const QVariant& var2) const {
  if (var1.type() == QVariant::Double && var2.type() == QVariant::Double) {
    return qFuzzyCompare(var1.toDouble(), var2.toDouble());
  }

  return var1 == var2;
}

}  // End namespace Core
}  // End namespace Updraft

