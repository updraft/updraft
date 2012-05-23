#include "translationlanguage.h"

namespace Updraft {
namespace Core {

TranslationLanguage::TranslationLanguage() {}

TranslationLanguage::TranslationLanguage(const QString &str)
  : QString(str) {}

QString TranslationLanguage::asQString() const {
  return *static_cast<const QString*>(this);
}

}  // End namespace Core
}  // End namespace Updraft

