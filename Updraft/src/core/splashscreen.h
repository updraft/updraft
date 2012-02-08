#ifndef UPDRAFT_SRC_CORE_SPLASHSCREEN_H_
#define UPDRAFT_SRC_CORE_SPLASHSCREEN_H_

#include <QtGui>

namespace Updraft {
namespace Core {

class SplashScreen: public QSplashScreen {
 public:
  SplashScreen(const QPixmap& pixmap = QPixmap());
 private:
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SPLASHSCREEN_H_

