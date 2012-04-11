#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_TASKPOINTBUTTON_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_TASKPOINTBUTTON_H_

#include <QFrame>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;

namespace Updraft {

class TaskPointButton: public QFrame {
 Q_OBJECT

 public:
  TaskPointButton(int pos, const QString& name);

  void setName(const QString& newName);
  QString getName();
 private:
  /// Position of the TaskPoint
  int taskPointPos;

  // Graphical user interface:
  // +--------------- this ----------------+
  // | number  nameWidget  +- closeFrame -+|
  // |                     |  quitButton  ||
  // |  "1."  "TextLabel"  |      [X]     ||
  // |                     +--------------+|
  // +-------------------------------------+
  QHBoxLayout* topFrameLayout;
  QLabel* number;
  QLabel* nameWidget;
  QFrame* closeFrame;
  QVBoxLayout* closeFrameLayout;
  QPushButton* quitButton;
};

}  // End namespace Updraft

#endif

