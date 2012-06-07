#ifndef UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGE_H_
#define UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGE_H_

#include <QString>
#include <QMetaType>

namespace Updraft {
namespace Core {

/// Helper class representing a selected translation language.
/// Needed to get a specialized editor for plain QString.
class TranslationLanguage: public QString {
 public:
  TranslationLanguage();
  explicit TranslationLanguage(const QString &str);

  /// Returns this translation language as a string value.
  /// \return The string representation of this translation language
  QString asQString() const;
};

}  // End namespace Core
}  // End namespace Updraft

Q_DECLARE_METATYPE(Updraft::Core::TranslationLanguage)

#endif  // UPDRAFT_SRC_CORE_TRANSLATIONLANGUAGE_H_

