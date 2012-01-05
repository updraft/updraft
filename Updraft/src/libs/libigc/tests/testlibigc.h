#ifndef UPDRAFT_SRC_LIBS_LIBIGC_TESTS_TESTLIBIGC_H_
#define UPDRAFT_SRC_LIBS_LIBIGC_TESTS_TESTLIBIGC_H_

#include <QObject>

namespace Updraft {
namespace Libigc {
namespace Test {

class TestLibigc: public QObject {
  Q_OBJECT
 private slots:
  void testHRecords();
};

}  // End namespace Test
}  // End namespace Libigc
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBS_LIBIGC_TESTS_TESTLIBIGC_H_

