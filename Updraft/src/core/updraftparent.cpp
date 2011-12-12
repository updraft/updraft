#include "updraftparent.h"

namespace Updraft {
namespace Core {

UpdraftParent::UpdraftParent(MainWindow* setWin)
: win(setWin), pluginManager(this) {}

}  // End namespace Core
}  // End namespace Updraft

