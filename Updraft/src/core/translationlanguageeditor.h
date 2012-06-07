#ifndef UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGEEDITOR_H_
#define UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGEEDITOR_H_

#include <QComboBox>

#include "translationlanguage.h"

namespace Updraft {
namespace Core {

/// Editor for selecting translation language in settings dialog.
/// Shows a combo box with all available languages.
class TranslationLanguageEditor: public QComboBox {
  Q_OBJECT
  Q_PROPERTY
    (Updraft::Core::TranslationLanguage lang READ lang WRITE setLang USER true)
 public:
  explicit TranslationLanguageEditor(QWidget *parent);

  /// Gets the translation language property stored in this editor.
  /// \return The translation language currently in the editor.
  TranslationLanguage lang();

  /// Sets the translation language property of the editor.
  /// \param l The translation language to be set.
  void setLang(const TranslationLanguage &l);
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGEEDITOR_H_

