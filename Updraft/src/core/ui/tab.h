#ifndef UPDRAFT_SRC_CORE_UI_TAB_H_
#define UPDRAFT_SRC_CORE_UI_TAB_H_

#include <QString>
#include <QWidget>

class QTabWidget;

namespace Updraft {

class PluginBase;

namespace Core {

class MainWindow;

class Tab : public QWidget {
  Q_OBJECT

 public:
  Tab(QWidget* content, QString title, QTabWidget* parent, PluginBase* owner);

  QWidget* getWidget() { return widget; }

 public slots:
  void close();

 private:
  void deselected();
  void selected();

  QWidget* widget;
  QTabWidget* tabWidget;
  PluginBase* plugin;

  friend class MainWindow;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_TAB_H_

