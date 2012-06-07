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

/// Implementation of an opened tab in the bottom pane.
class Tab : public QWidget, public TabInterface {
  Q_OBJECT

 public:
  Tab(QWidget* content, QString title, QTabWidget* parent);
  ~Tab();

  QWidget* getWidget() { return widget; }

  void select();
  void setTitle(const QString &title);

  void connectSlotClose(const QObject* sender, const char *signal);
  void connectSignalCloseRequested(const QObject* receiver,
    const char *method);
  void connectCloseRequestToClose();
  void connectSignalSelected(const QObject* receiver, const char *method);
  void connectSignalDeselected(const QObject* receiver, const char *method);

 public slots:
  void close();

 signals:
  void tabCloseRequested();
  void tabRemoved(QWidget *tab);
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

