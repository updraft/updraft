#include "updraft.h"

int main(int argc, char *argv[]) {
  Updraft::Core::Updraft app(argc, argv);
  return app.exec();
}
