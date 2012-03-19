#ifndef UPDRAFT_SRC_LIBRARIES_IGC_TESTS_WRONG_WRONG_H_
#define UPDRAFT_SRC_LIBRARIES_IGC_TESTS_WRONG_WRONG_H_

#include <QObject>

namespace Updraft {
namespace Igc {
namespace Test {

/// Test invalid igc files.
class Wrong: public QObject {
  Q_OBJECT
 private slots:
  void a_record();
  void b_record();
};

}  // End namespace Test
}  // End namespace Igc
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_IGC_TESTS_WRONG_WRONG_H_

