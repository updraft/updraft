#include "translationlanguageeditor.h"

#include "updraft.h"
#include "translationmanager.h"

namespace Updraft {
namespace Core {

TranslationLanguageEditor::TranslationLanguageEditor(QWidget *parent)
  : QComboBox(parent) {
  addItems(updraft->translationManager->availableLanguages());
}

TranslationLanguage TranslationLanguageEditor::lang() {
  return TranslationLanguage(currentText());
}

void TranslationLanguageEditor::setLang(const TranslationLanguage &l) {
  for (int i = 0; i < count(); ++i) {
    if (itemText(i) == l) {
      setCurrentIndex(i);
      return;
    }
  }

  addItem(l.asQString());
  setCurrentIndex(count() - 1);
}

}  // End namespace Core
}  // End namespace Updraft

