#ifndef UPDRAFT_CORE_INTERFACE_H_
#define UPDRAFT_CORE_INTERFACE_H_

namespace Updraft {

//Forward declarations
class IPlugin;
namespace Core{
  class UpdraftParent;
} //End namespace Core

class CoreInterface {
  /// \todo Fill this once plugin API is finished
 public:
  CoreInterface(): plugin(NULL), parent(NULL) {}
  CoreInterface(Core::UpdraftParent* par, IPlugin *pl) :
    plugin(pl), parent(par) {}

  /// Checks, whether the class was correctly constructed.
  bool valid() { return (plugin != NULL && parent != NULL); }

 private:
  IPlugin* plugin;
  Core::UpdraftParent* parent;
};

}  //End namespace Updraft

#endif

