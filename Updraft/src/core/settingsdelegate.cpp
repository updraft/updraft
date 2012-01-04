#include "settingsdelegate.h"

namespace Updraft {
namespace Core {

void SettingsDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
  QByteArray propertyName = editor->metaObject()->userProperty().name();
  qDebug() << propertyName;

  // Only set the value if the property differs from the value in the model
  if (!variantsEqual(model->data(index, Qt::EditRole), editor->property(propertyName))) {
    QStyledItemDelegate::setModelData(editor, model, index);
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

