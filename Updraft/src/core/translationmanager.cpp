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
  coreTranslator = new QTranslator(this);
  updraft->installTranslator(coreTranslator);

  foreach(PluginBase *plugin, updraft->pluginManager->getAllPlugins()) {
    QTranslator *translator = new QTranslator(this);
    pluginTranslators.append(PluginTranslator(plugin, translator));
    updraft->installTranslator(translator);
  }

  QVariant defaultValue;
  defaultValue.setValue(TranslationLanguage(defaultLanguage));

  languageSetting = updraft->settingsManager->addSetting(
    "core:language",
    "",  // description is given after a language file is loaded
    defaultValue,
    false);

  QString language =
    languageSetting->get().value<TranslationLanguage>().asQString();
  loadLanguage(language);

  languageSetting->setDescription(tr("Language"));
}

void TranslationManager::loadTranslationFile(
  QTranslator *translator, const QDir &dir, const QString &lang) {
  QString path = dir.absoluteFilePath(lang + ".qm");
  if (!translator->load(path)) {
    qDebug() << "Loading translation file " << path << " failed.";
  }
}

void TranslationManager::loadLanguage(const QString &lang) {
  qDebug() << "Loading language " << lang;

  if (lang == defaultLanguage) {
    return;
  }

  QDir dir = updraft->getTranslationDirectory();
  loadTranslationFile(coreTranslator, dir, lang);

  foreach(PluginTranslator pair, pluginTranslators) {
    QString name = pair.first->getName();
    dir = updraft->pluginManager->getPluginDir(name);
    loadTranslationFile(pair.second, dir, lang);
  }
}

QStringList TranslationManager::availableLanguages() {
  QStringList nameFilter;
  nameFilter << "*.qm";

  QSet<QString> found;

  found.unite(
    updraft->getTranslationDirectory().entryList(
      nameFilter, QDir::Files | QDir::Readable).toSet());

  foreach(PluginTranslator pair, pluginTranslators) {
    QString name = pair.first->getName();
    QDir dir = updraft->pluginManager->getPluginDir(name);

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

