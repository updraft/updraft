#include "translationmanager.h"

#include <QLocale>

#include "updraft.h"
#include "translationlanguage.h"
#include "../pluginbase.h"
#include "pluginmanager.h"
#include "settingsmanager.h"

namespace Updraft {
namespace Core {

const QString TranslationManager::defaultLanguage = "english";

TranslationManager::TranslationManager() {
  QVariant defaultValue;
  defaultValue.setValue(TranslationLanguage(defaultLanguage));

  languageSetting = updraft->settingsManager->addSetting(
    "general:language",
    "",  // description is given after a language file is loaded
    defaultValue,
    false);

  addTranslations(updraft->getTranslationDirectory());

  languageSetting->setDescription(tr("Language"));
}

void TranslationManager::addTranslations(const QDir &dir) {
  QString language =
    languageSetting->get().value<TranslationLanguage>().asQString();

  QTranslator *translator = new QTranslator(this);
  dirTranslators.append(DirTranslator(dir, translator));

  if (language == defaultLanguage) {
    return;
  }

  QString path = dir.absoluteFilePath(language + ".qm");
  if (!translator->load(path)) {
    qDebug() << "Loading translation file " << path << " failed.";
  }

  updraft->installTranslator(translator);
}

QStringList TranslationManager::availableLanguages() {
  QStringList nameFilter;
  nameFilter << "*.qm";

  QSet<QString> found;

  foreach(DirTranslator pair, dirTranslators) {
    QDir dir = pair.first;

    found.unite(dir.entryList(
      nameFilter, QDir::Files | QDir::Readable).toSet());
  }

  QStringList ret;

  ret.append(defaultLanguage);

  foreach(QString str, found) {
    ret.append(str.left(str.size() - 3));
  }

  return ret;
}

}  // End namespace Core
}  // End namespace Updraft

