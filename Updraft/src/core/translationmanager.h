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
  void translate();

  /// List all available languages
  QStringList availableLanguages();

 private slots:
  /// Called when the language setting changes.
  /// Sets the language and calls the translate() method on
  /// updraft singleton (= the whole application).
  void languageChanged();

 private:
  /// Load all the languages to the translator.
  void loadLanguage(const QString &lang);

  /// Attempt to load a single translation file.
  void loadTranslationFile(QTranslator *translator,
    const QDir &dir, const QString &lang);

  typedef QPair<PluginBase *, QTranslator *> PluginTranslator;

  QList<PluginTranslator> pluginTranslators;
  QTranslator *coreTranslator;

  SettingInterface *languageSetting;

  static const QString defaultLanguage;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_TRANSLATIONMANAGER_H_

