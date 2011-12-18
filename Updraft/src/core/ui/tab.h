#ifndef UPDRAFT_SRC_CORE_UI_TAB_H_
#define UPDRAFT_SRC_CORE_UI_TAB_H_

#include <QString>
#include <QWidget>

#include "../../tabinterface.h"

class QTabWidget;

namespace Updraft {

class PluginBase;

namespace Core {

class MainWindow;

class Tab : public QWidget, public TabInterface {
  Q_OBJECT

 public:
  Tab(QWidget* content, QString title, QTabWidget* parent);
  ~Tab();

  QWidget* getWidget() { return widget; }

  void connectSlotClose(const QObject* sender, const char *signal);
  void connectSignalClosed(const QObject* receiver, const char *method);
  void connectSignalSelected(const QObject* receiver, const char *method);
  void connectSignalDeselected(const QObject* receiver, const char *method);

 public slots:
  void close();

 signals:
  void closed();
  void selected();
  void deselected();

 private:
  QWidget* widget;
  QTabWidget* tabWidget;
  PluginBase* plugin;

  friend class MainWindow;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_UI_TAB_H_

