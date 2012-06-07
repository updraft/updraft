#ifndef UPDRAFT_SRC_CORE_SPLASHSCREEN_H_
#define UPDRAFT_SRC_CORE_SPLASHSCREEN_H_

#include <QtGui>

namespace Updraft {
namespace Core {

/// Startup splash window.
class SplashScreen: public QSplashScreen {
 public:
  /// Creates the splash screen instance.
  SplashScreen();
 private:
};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_CORE_SPLASHSCREEN_H_

