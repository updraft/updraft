#include "translationmanager.h"

#include <QLocale>

#include "updraft.h"
#include "../pluginbase.h"

namespace Updraft {
namespace Core {

TranslationManager::TranslationManager() {
  coreTranslator = new QTranslator(this);
  updraft->installTranslator(coreTranslator);

  foreach(PluginBase *plugin, updraft->pluginManager->getAllPlugins()) {
    QTranslator *translator = new QTranslator(this);
    pluginTranslators.append(PluginTranslator(plugin, translator));
    updraft->installTranslator(translator);
  }

  languageSetting = updraft->settingsManager->addSetting(
    "core:language",
    tr("Language"),
    QLocale::languageToString(QLocale::system().language()),
      // TODO(Kuba) Create an editor for trasnlations.
    false);
  languageSetting->callOnValueChanged(this, SLOT(languageChanged()));

  languageChanged();

  // TODO(Kuba): Translate the setting description
  // languageSetting->setDescription()
}

void TranslationManager::languageChanged() {
  QString language = languageSetting->get().value<QString>();

  loadLanguage(language);
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

  QDir dir = updraft->getTranslationDirectory();
  loadTranslationFile(coreTranslator, dir, lang);

  foreach(PluginTranslator pair, pluginTranslators) {
    QString name = pair.first->getName();
    dir = updraft->pluginManager->getPluginDir(name);
    loadTranslationFile(pair.second, dir, lang);
  }
}

}  // End namespace Core
}  // End namespace Updraft

