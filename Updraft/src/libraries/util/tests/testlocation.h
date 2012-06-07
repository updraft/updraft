#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_TESTS_TESTLOCATION_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_TESTS_TESTLOCATION_H_

#include <QObject>

#include "location.h"

namespace Updraft {
namespace Util {
namespace Test {

/// Test operations on Location object.
class TestLocation: public QObject {
  Q_OBJECT
 private slots:
  void testLat();
  void testLon();
};

}  // End namespace Test
}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_TESTS_TESTLOCATION_H_

