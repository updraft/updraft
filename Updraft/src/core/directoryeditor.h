#ifndef UPDRAFT_SRC_CORE_DIRECTORYEDITOR_H_
#define UPDRAFT_SRC_CORE_DIRECTORYEDITOR_H_

#include <QPushButton>
#include <QDir>

namespace Updraft {
namespace Core {

/// Editor for QDir in settings dialog.
/// Displays a button that opens standard directory select dialog when clicked.
class DirectoryEditor: public QPushButton {
  Q_OBJECT
  Q_PROPERTY(QDir directory READ directory WRITE setDirectory USER true)
 public:
  explicit DirectoryEditor(QWidget *parent = 0);

  /// Gets the directory property value of this editor.
  /// \return The curently selected directory in the editor.
  const QDir& directory() const;

  /// Sets the directory property displayed in the editor.
  /// \param dir The directory to be displayed in the editor.
  void setDirectory(const QDir &dir);

 private slots:
  /// Reacts on the button click and displays the standard directory selection
  /// dialog that allows the user to change the directory property.
  void onClick();

 private:
  /// The property of this editor
  QDir dirProp;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_DIRECTORYEDITOR_H_

