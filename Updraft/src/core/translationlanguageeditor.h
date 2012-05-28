#ifndef UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGEEDITOR_H_
#define UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGEEDITOR_H_

#include <QComboBox>

#include "translationlanguage.h"

namespace Updraft {
namespace Core {

class TranslationLanguageEditor: public QComboBox {
  Q_OBJECT
  Q_PROPERTY
    (Updraft::Core::TranslationLanguage lang READ lang WRITE setLang USER true)
 public:
  explicit TranslationLanguageEditor(QWidget *parent);

  TranslationLanguage lang();
  void setLang(const TranslationLanguage &l);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGEEDITOR_H_

