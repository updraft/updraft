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

  /// Gets the color property from this editor
  /// \return The color in this editor
  const QColor &color();

  /// Sets the color property of this editor
  /// \param c The color to set
  void setColor(const QColor &c);

 private slots:
  /// Reacts to the click on the editor's button. Creates a color-selection
  /// dialog and lets the user select the color using it.
  void onClick();

 private:
  /// The color property of this editor
  QColor c;

  /// The displayed button of this editor
  QPushButton* button;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_COLOREDITOR_H_

