#ifndef UPDRAFT_SRC_CORE_COLOREDITOR_H_
#define UPDRAFT_SRC_CORE_COLOREDITOR_H_

#include <QWidget>

class QPushButton;

namespace Updraft {
namespace Core {

/// Editor for QColor in settings dialog.
/// Displays a button that opens standard color dialog when clicked.
class ColorEditor: public QWidget {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor USER true)
 public:
  explicit ColorEditor(QWidget *parent = 0);

  const QColor &color();
  void setColor(const QColor &c);

 private slots:
  void onClick();

 private:
  QColor c;
  QPushButton* button;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_COLOREDITOR_H_

