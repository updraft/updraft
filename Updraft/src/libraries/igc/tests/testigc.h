#ifndef UPDRAFT_SRC_LIBRARIES_IGC_TESTS_TESTIGC_H_
#define UPDRAFT_SRC_LIBRARIES_IGC_TESTS_TESTIGC_H_

#include <QObject>

#include "igc.h"

namespace Updraft {
namespace Igc {
namespace Test {

class TestIgc: public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();

  void testHRecords();
  void testBRecords();

  void testClean();

 private:
  void loadTestFile();
  Updraft::Igc::IgcFile igc;
};

}  // End namespace Test
}  // End namespace Igc
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_IGC_TESTS_TESTIGC_H_

