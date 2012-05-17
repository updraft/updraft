#ifndef UPDRAFT_SRC_CORE_VARIANTFUNCTIONS_H_
#define UPDRAFT_SRC_CORE_VARIANTFUNCTIONS_H_

#include <QtGui>
#include <QMetaType>

Q_DECLARE_METATYPE(QDir);

namespace Updraft {
namespace Core {

bool variantsEqual(const QVariant& var1, const QVariant& var2);

/// This function is defined so that we can control conversion to
/// a string for any type that a variant can hold.
QString variantToString(const QVariant& var);

QVariant variantFromString(const QString& data, const QString& typeName);

void registerMetaTypes();

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_VARIANTFUNCTIONS_H_

