#ifndef UPDRAFT_SRC_CORE_UI_TAB_H_
#define UPDRAFT_SRC_CORE_UI_TAB_H_

#include <QString>
#include <QWidget>

class QTabWidget;

namespace Updraft {

class IPlugin;

namespace Core {

class MainWindow;

class Tab : public QWidget {
  Q_OBJECT

 public:
  Tab(QWidget* content, QString title, QTabWidget* parent, IPlugin* owner);

  QWidget* getWidget() { return widget; }

 public slots:
  void close();

 private:
  void deselected();
  void selected();

  QWidget* widget;
  QTabWidget* tabWidget;
  IPlugin* plugin;

  friend class MainWindow;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_TAB_H_

