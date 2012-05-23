#include "translationlanguageeditor.h"

#include <QDebug>

#include "updraft.h"

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
  qDebug() << "TranslationLanguageEditor::setLang";
  QString l2 = l.asQString();
  for (int i = 0; i < count(); ++i) {
    if (itemText(i) == l2) {
      setCurrentIndex(i);
      qDebug() << l2 << "->" << currentText();
      return;
    }
  }

  addItem(l2);
  setCurrentIndex(count() - 1);
}

}  // End namespace Core
}  // End namespace Updraft

