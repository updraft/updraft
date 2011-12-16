#include "tab.h"

#include <QTabWidget>
#include <QHBoxLayout>

#include "../../pluginbase.h"

namespace Updraft {
namespace Core {

/// Close this tab.
/// This method deletes the tab and its widget.
void Tab::close() {
  plugin->tabClosed(this);

  int id = tabWidget->indexOf(this);
  tabWidget->removeTab(id);

  delete this;
}

/// Add a new tab to parent.
/// Takes ownership of content.
Tab::Tab(QWidget* content, QString title, QTabWidget* parent, PluginBase* owner)
  : widget(content), tabWidget(parent), plugin(owner) {
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->addWidget(widget);

  tabWidget->addTab(this, title);
}

void Tab::deselected() {
  plugin->tabDeselected(this);
}

void Tab::selected() {
  plugin->tabSelected(this);
}

}  // End namespace Core
}  // End namespace Updraft

