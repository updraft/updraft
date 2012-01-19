#include "testlocation.h"

#include <QtTest>

namespace Updraft {
namespace Util {
namespace Test {

void TestLocation::testLat() {
  Location l;

  l.latFromDMS(0, 0, 0, 'N');
  QCOMPARE(l.lat, 0.0);

  l.latFromDMS(50, 6, 58.378, 'N');
  QCOMPARE(l.lat, 50.116216111111115);

  l.latFromDMS(50, 6, 58.378, 'S');
  QCOMPARE(l.lat, -50.116216111111115);
}

void TestLocation::testLon() {
  Location l;

  l.lonFromDMS(0, 0, 0, 'E');
  QCOMPARE(l.lon, 0.0);

  l.lonFromDMS(50, 6, 58.378, 'E');
  QCOMPARE(l.lon, 50.116216111111115);

  l.lonFromDMS(50, 6, 58.378, 'W');
  QCOMPARE(l.lon, -50.116216111111115);
}

}  // End namespace Test
}  // End namespace Util
}  // End namespace Updraft

QTEST_MAIN(Updraft::Util::Test::TestLocation)
