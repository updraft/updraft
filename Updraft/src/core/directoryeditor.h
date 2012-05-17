#ifndef UPDRAFT_SRC_CORE_DIRECTORYEDITOR_H_
#define UPDRAFT_SRC_CORE_DIRECTORYEDITOR_H_

#include <QPushButton>
#include <QDir>

namespace Updraft {
namespace Core {

class DirectoryEditor: public QPushButton {
  Q_OBJECT
  Q_PROPERTY(QDir directory READ directory WRITE setDirectory USER true)
 public:
  explicit DirectoryEditor(QWidget *parent = 0);

  const QDir& directory() const;
  void setDirectory(const QDir &dir);

 private slots:
  void onClick();

 private:
  QDir dirProp;
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_DIRECTORYEDITOR_H_

