#ifndef UPDRAFT_SRC_CORE_TRANSLATIONMANAGER_H_
#define UPDRAFT_SRC_CORE_TRANSLATIONMANAGER_H_

#include <QList>
#include <QString>
#include <QPair>
#include <QStringList>
#include <QTranslator>

#include "../settinginterface.h"
#include "../pluginbase.h"

namespace Updraft {
namespace Core {

class TranslationManager: public QObject {
  Q_OBJECT

 public:
  TranslationManager();

  /// List all available languages
  QStringList availableLanguages();

  /// Add a directory with translation and attempt to load a
  /// language from it.
  void addTranslations(const QDir &dir);
 private:
  typedef QPair<QDir, QTranslator*> DirTranslator;

  QList<DirTranslator> dirTranslators;

  SettingInterface *languageSetting;

  static const QString defaultLanguage;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_TRANSLATIONMANAGER_H_

