#include "variantfunctions.h"
#include "translationlanguage.h"
#include "ellipsoidname.h"

namespace Updraft {
namespace Core {

bool variantsEqual(const QVariant& var1, const QVariant& var2) {
  int type1 = QMetaType::type(var1.typeName());
  // int type2 = QMetaType::type(var2.typeName());

  if (type1 == QMetaType::type("double")) {
    return qFuzzyCompare(var1.toDouble(), var2.toDouble());
  } else if (type1 == QMetaType::type("QDir")) {
    return
      var1.value<QDir>().absolutePath() == var2.value<QDir>().absolutePath();
  } else if (type1 ==  QMetaType::type("TranslationLanguage")) {
    return var1.value<TranslationLanguage>().asQString() ==
      var2.value<TranslationLanguage>().asQString();
  } else if (type1 ==  QMetaType::type("EllipsoidName")) {
    return var1.value<EllipsoidName>().asQString() ==
      var2.value<EllipsoidName>().asQString();
  } else {
    return var1 == var2;
  }
}

QString variantToString(const QVariant& var) {
  int type = QMetaType::type(var.typeName());

  if (type == QMetaType::type("QDir")) {
    return var.value<QDir>().absolutePath();
  } else if (type == QMetaType::type("TranslationLanguage")) {
    return var.value<TranslationLanguage>().asQString();
  } else if (type == QMetaType::type("EllipsoidName")) {
    return var.value<EllipsoidName>().asQString();
  } else if (type == QMetaType::type("QByteArray")) {
    return var.toByteArray().toBase64();
  } else {
    return var.toString();
  }
}

QVariant variantFromString(const QString& data, const QString& typeName) {
  int type = QMetaType::type(typeName.toAscii().data());
  QVariant ret;

  if (type == QMetaType::type("QDir")) {
    ret.setValue(QDir(data));
  } else if (type == QMetaType::type("TranslationLanguage")) {
    ret.setValue(TranslationLanguage(data));
  } else if (type == QMetaType::type("EllipsoidName")) {
    ret.setValue(EllipsoidName(data));
  } else if (type == QMetaType::type("QByteArray")) {
    ret.setValue(QByteArray::fromBase64(data.toAscii()));
  } else {
    ret.setValue(data);
    ret.convert(QVariant::nameToType(typeName.toAscii().data()));
  }

  return ret;
}

void registerMetaTypes() {
  qRegisterMetaType<QDir>("QDir");
  qRegisterMetaType<TranslationLanguage>("TranslationLanguage");
  qRegisterMetaType<EllipsoidName>("EllipsoidName");
}

}  // End namespace Core
}  // End namespace Updraft

